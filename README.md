# ShellColor

##### A Simple ASCII Color Generation C++ Library & Command-Line Application.

## C++ Library

The Library is independent and Only Supports Linux Shell and Most of its Terminal Emulators.

### Overview

The Main Header File has Three (3) Main Namespaces:

| Namespace Name | Description                                                                                       |
|:--------------:|:-------------------------------------------------------------------------------------------------:|
| Color          | Stores Escape Codes for **Background Colors, Foreground Colors, and Styles**.                     |
| colormerge     | Stores Essential Functions for Main Operations such as Print, Manipulating Escape Sequence Codes. |
| colorformat    | Stores Format/Translate Operation Related Functions                                               |

#### Details

The Namespace "Color" contains Three Inner Namespaces which are **Back (Color::Back), Fore (Color::Fore), and Style (Color::Style)**, and **Color Map** (Color::Map) which is an **std::map\<std::string, const char>**

- The Namespace **Back** stores Background Escape Colors.

- The Namespace **Fore** stores Foreground Escape Colors, 

- The Namespace **Style** stores Escape Style Codes. Which Are:

- ###### Namespace "Color" inner Variables:
  
  | Variable  | Description                                                              |
  |:---------:|:------------------------------------------------------------------------:|
  | PALETTE   | Escape Sequence Template.                                                |
  | RESET_ALL | Reset Code For Colors and Styles.                                        |
  | MAP       | A Map which links Fore, Back, Style Namespaces inner Codes into strings. |

##### Important Functions

-----------

```cpp
std::string colormerge::inpalette(std::string input, std::string palette)
```

- **Description:** Generates a valid escape sequence according to a palette (Escape Sequence Template) by Replacing **The Braces {}** which indicate the position in the string "palette" to merge the "**input**" to the "**palette**"

- **Parameters**:
  
  - **std::string input:** String value to be merged (replace in) into **palette (Parameter)**.
  
  - **std::string palette:** (**Template**) string value to be merged with **input (Parameter)**.

- **Palette (Escape Sequence Template):** **\e[{}m

- **

- **Example**: **colormerge::inpalette("COLOR", Color::PALETTE)**

- **Example Return:** "\e[**COLOR**m"

---

```cpp
std::string colormerge::merge(std::vector<char> codes)
```

- **Description:** merges every item in **codes (std::vector\<char> Parameter)** into a single string.

- **Parameters:**
  
  - **std::vector\<char> codes:** Escape Codes to be merged.

- **Example**: **colormerge::merge({40, 31, 1})**

- **Example Return:** "**40;31;1**"

---

```cpp
std::string colormerge::generate(std::vector<char> codes, std::string input, std::string palette, bool reset=true)
```

- **Description:** generates an Escape sequence according the parameters (codes, input, palette, reset), and If **reset (bool Parameter)** is true, the Escape Code **Color::RESET_ALL** *(Resets (Background and Foreground)* Colors, Style) will be added to the End of the Escape Sequence.

- **Parameters:**
  
  - **std::vector\<char> codes:** Escape Codes to be added to The Escape Sequence.
  
  - **std::string input:** String Value to be Colorized **(Added to The Escape Sequence)**.
  
  - **std::string palette:** (**Template**) string value to be merged with **input (Parameter)**.
  
  - **bool reset=true:** **Resets (Background and Foreground) Colors and Style** at the End Of The Escape Sequence.

---

```cpp
void colormerge::printc(std::string input, std::vector<char> codes, bool reset=true, std::string endl="\n")
```

- **Description:** prints an Escape Sequence which is Generated from The Parameters **(input, codes, reset, endl)

- **Parameters:**
  
  - **std::string input:** String Value to be Colorized **(Added to The Escape Sequence)**.
  
  - **std::vector<char> codes:** Escape Codes to be added to The Escape Sequence.
  
  - **bool reset=true:** **Resets (Background and Foreground) Colors and Style** at the End Of The Escape Sequence.
  
  - **std::string endl:** <u>String Value to be added to the End Of The Escape Sequence</u> after The Reset Escape code **(if the parameter "reset" is true)**

---

```cpp
std::vector<int> colorformat::find(std::string input)
```

- **Description:** Locates format-able parts positions in **Parameter "input"**.
- **Parameters:**
  - **std::string input:** String Value to find in.

---

```cpp
std::string colorformat::parse(std::string input)
```

- **Description:** translates multiple colors in single Code Indicator ***{\__:\_\_:\__}** to an Escape Sequence.

- **Parameters:**
  
  - **std::string input:** A String Value which contains a single Code Indicator to be translated into an Escape Sequence.

---

```cpp
std::string colorformat::format(std::string input)
```

- **Description:** Fully Parses and Translates Color Indicators in **The Parameter "input"** into a consistent escape sequence text.

- **Parameter:**
  
  - **std::string input:** A String Value which contains Code Indicator to be translated into an Escape Sequence.

- **Example:** \*{FMAGENTA:BBLACK}Hello World!*{RESET_ALL}

- **Example Deduces:** \x1b[35;40mHello World!\x1b[0m

---

#### Examples

**colormerge::printc**, Printing MAGENTA-BLACK-Underlined **Hello World !**

```cpp
#include <iostream>
#include "shellcolor.h"

int main(int argc, char ** argv) {
    colormerge::printc("Hello World !",
                       {Color::Fore::MAGENTA,
                       Color::Back::BLACK,
                       Color::Style::UNDERLINE});
    return 0;
}
```

- **Raw (Unformatted) Output:** **\e[35;40;4mHello World !\e[0m**

---------------------

**colorformat::format**, Formatting **\*{FMAGENTA:BBLACK:UNDERLINE}Hello World!*{RESET_ALL}**  to an Escape Sequence.

```cpp
#include <iostream>
#include "shellcolor.h"

int main(int argc, char ** argv) {
    std::string escape_sequence = colorformat::format("*{FMAGENTA:BBLACK:UNDERLINE}Hello World !*{RESET_ALL}");
    std::cout << escape_sequence << std::endl;
    return 0;
}
```

- **Value of Variable "escape_sequence":**  **\e[35;40;4mHello World !\e[0m**

------

**colormerge::generate**, Generating an Escape Sequence in form of MAGENTA-BLACK-Underlined **Hello World !** text.

```cpp
#include <iostream>
#include <shellcolor.h>

int main(int argc, char ** argv) {
    std::string generated = colormerge::generate({35, 40, 4}, "Hello World !", Color::PALETTE)
}
```

- **Value of Variable "generated":** **\e[35;40;4mHello World !\e[0m

----

#### Additional Information

###### Color Codes RAPIDBLINK, ITALIC are not widely supported.

## License

Shell-Colors and all its components are Licensed Under [GNU General Public License Version 3](https://www.gnu.org/licenses/gpl-3.0.en.html)
