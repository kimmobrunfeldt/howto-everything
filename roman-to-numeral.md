
# Convert number to roman numerals

```python
VALUES = [1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1]
NUMERALS = ["M", "CM", "D", "CD", "C", "XC", "L",
            "XL", "X", "IX", "V", "IV", "I"]


def int_to_roman(value):
    """
    Example: value=1300

    1101 / 1000 = 1 -> add M, 1101 - 1000 = 101

    101 / 1000 = 0
    101 / 900 = 0
    101 / 500 = 0
    101 / 400 = 0
    101 / 100 = 1 -> add C, 101 - 100 = 1

    1 / 1000 = 0
    1 / 900 = 0
    1 / 500 = 0
        ...
    1 / 4 = 0
    1 / 1 = 1 -> add I, 1 - 1 = 0, FINISHED

    Numerals are "MCI".
    """
    roman_number = ''
    index = 0
    while value > 0:
        if value / VALUES[index] != 0:
            roman_number += NUMERALS[index]  # Add numeral to string
            value -= VALUES[index]
            index = 0  # Start from the largest value again.
        else:
            index += 1
    return roman_number

def roman_to_int(roman):
    value = 0
    for i, x in enumerate(NUMERALS):
        while roman.startswith(x):
            value += VALUES[i]
            roman = roman[len(x):]
    return value


if __name__ == '__main__':
    print int_to_roman(int(raw_input('Insert int: ')))
```
