#include "main.h"
unsigned char _handleflags(const char *flag, char *index);
unsigned char _handlelen(const char *modifier, char *index);
int _handlewidth(va_list args, const char *modifier, char *index);
int _handleprecision(va_list args, const char *modifier, char *index);
unsigned int (*_handlespecifiers(const char *specifier))(va_list, buffer_t *,
unsigned char, int, int, unsigned char);
/**
* _handleflags - Matches flags with corresponding values.
* @flag: A pointer to a potential string of flags.
* @index: An index counter for the original format string.
*
* Return: If flag characters are matched - a corresponding value.
*         Otherwise - 0.
*/
unsigned char _handleflags(const char *flag, char *index)
{
int i, j;
unsigned char ret = 0;
flag_t flags[] = {
{'+', PLUS},
{' ', SPACE},
{'#', HASH},
{'0', ZERO},
{'-', NEG},
{0, 0}
};
for (i = 0; flag[i]; i++)
{
for (j = 0; flags[j].flag != 0; j++)
{
if (flag[i] == flags[j].flag)
{
(*index)++;
if (ret == 0)
ret = flags[j].value;
else
ret |= flags[j].value;
break;
}
}
if (flags[j].value == 0)
break;
}
return (ret);
}
/**
* _handlelen - Matches length modifiers with their corresponding value.
* @modifier: A pointer to a potential length modifier.
* @index: An index counter for the original format string.
*
* Return: If a lenth modifier is matched - its corresponding value.
*         Otherwise - 0.
*/
unsigned char _handlelen(const char *modifier, char *index)
{
if (*modifier == 'h')
{
(*index)++;
return (SHORT);
}
else if (*modifier == 'l')
{
(*index)++;
return (LONG);
}
return (0);
}
/**
* _handlewidth - Matches a width modifier with its corresponding value.
* @args: A va_list of arguments.
* @modifier: A pointer to a potential width modifier.
* @index: An index counter for the original format string.
*
* Return: If a width modifier is matched - its value.
*         Otherwise - 0.
*/
int _handlewidth(va_list args, const char *modifier, char *index)
{
int value = 0;
while ((*modifier >= '0' && *modifier <= '9') || (*modifier == '*'))
{
(*index)++;
if (*modifier == '*')
{
value = va_arg(args, int);
if (value <= 0)
return (0);
return (value);
}
value *= 10;
value += (*modifier - '0');
modifier++;
}
return (value);
}
/**
* _handleprecision - Matches a precision modifier with
*                    its corresponding value.
* @args: A va_list of arguments.
* @modifier: A pointer to a potential precision modifier.
* @index: An index counter for the original format string.
*
* Return: If a precision modifier is matched - its value.
*         If the precision modifier is empty, zero, or negative - 0.
*         Otherwise - -1.
*/
int _handleprecision(va_list args, const char *modifier, char *index)
{
int value = 0;
if (*modifier != '.')
return (-1);
modifier++;
(*index)++;
if ((*modifier <= '0' || *modifier > '9') &&
*modifier != '*')
{
if (*modifier == '0')
(*index)++;
return (0);
}
while ((*modifier >= '0' && *modifier <= '9') ||
(*modifier == '*'))
{
(*index)++;
if (*modifier == '*')
{
value = va_arg(args, int);
if (value <= 0)
return (0);
return (value);
}
value *= 10;
value += (*modifier - '0');
modifier++;
}
return (value);
}
/**
* _handlespecifiers - Matches a conversion specifier with
*                     a corresponding conversion function.
* @specifier: A pointer to a potential conversion specifier.
*
* Return: If a conversion function is matched - a pointer to the function.
*         Otherwise - NULL.
*/
unsigned int (*_handlespecifiers(const char *specifier))(va_list, buffer_t *,
unsigned char, int, int, unsigned char)
{
int i;
converter_t converters[] = {
{'c', _convertchar},
{'s', int_converttostring},
{'d', _converttosignedint},
{'i', _converttosignedint},
{'%', _convertpercent},
{'b', _converttobinary},
{'u', _converttodecimal},
{'o', _converttooctal},
{'x', _convertunsignedtohex},
{'X', _convertunsignedtoupperhex},
{'S', int_converttostrn},
{'p', _convertpointertohex},
{'r', _converttorev},
{'R', int_converttoRot13},
{0, NULL}
};
for (i = 0; converters[i].func; i++)
{
if (converters[i].specifier == *specifier)
return (converters[i].func);
}
return (NULL);
}
