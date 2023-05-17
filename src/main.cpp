/* a simple escape sequence generation command-line Application
 *
 * ShellColor and all of its components are Licensed under "GNU General Public License Version 3".
 *
 */

#include <iostream>
#include "shellcolor.h"
#include "shellargs.h"

// "APPNAME" is Application Name which will be used in help() function
#define APPNAME "shellcolor"
#define APPVERSION 0.9

using namespace std;

class App {
public:
    ArgsParser argsparser;
    int Run(int argc, char ** argv) {
        // Run: initial operation such as arguments parsing, and requests executing
        argsparser.add_option("text", "string", "", "-t", "--text");
        argsparser.add_option("background", "string", "NULL", "-b", "--background");
        argsparser.add_option("foreground", "string", "NULL", "-f", "--foreground");
        argsparser.add_option("style", "string", "NULL", "-s", "--style");
        argsparser.add_option("format", "string", "", "-fm", "--format");
        argsparser.add_option("list", "bool", "", "-l", "--list");
        argsparser.add_option("no_reset", "bool", "0", "-nr", "--no_reset");
        argsparser.add_option("no_newline", "bool", "0", "-nl", "--no_newline");
        argsparser.add_option("help", "bool", "0", "-h", "--help");
        argsparser.add_option("version", "bool", "0", "-v", "--version");
        argsparser.parse(argc, argv);

        /* Checks if help flag is present,
         * if it is, help message will be printed and other options will be ignored
         */
        if (argsparser.data["help"] == "1" || argc<=1 || argsparser.odd_args.size()-1 > 0) help();
        else main();

        if (argc<=1 || argsparser.odd_args.size()-1 > 0) return 1;

        return 0;
    }

    void main() {
        if (argsparser.data["version"] == "1") version();
        if (argsparser.data["list"] == "1") {
            colormerge::printc("Available Color: ", {Color::Fore::BLUE, Color::Style::BRIGHT}, true, "\n\n");
            for (std::pair<std::string, char> code : Color::Map) {
                printf("%s",
                       ("\t- " + code.first + " : " + colormerge::generate({code.second}, code.first, Color::PALETTE) + "\n").c_str());
            }
        }
        else if (argsparser.data["format"] != "")
                // Priority to "format" operation, when it is request operations other will be ignored
                printf("%s",
                       ((colorformat::format(argsparser.data["format"])+(argsparser.data["no_newline"]=="1"?"":"\n"))).c_str());

            else {
                std::vector<char> codes;
                if (argsparser.data["background"] != "NULL") codes.emplace_back(Color::Map["B" + argsparser.data["background"]]);
                if (argsparser.data["foreground"] != "NULL") codes.emplace_back(Color::Map["F" + argsparser.data["foreground"]]);
                codes = colorformat::pass_arg(codes, argsparser.data["style"]);
                // Prints colored text according to text value and other arguments
                if (argsparser.data["text"] != "") {
                    colormerge::printc(
                        argsparser.data["text"],
                        {codes},
                        (argsparser.data["no_reset"]=="1"?false:true),
                        (argsparser.data["no_newline"]=="1"?"":"\n")
                    );
                }
            }
    }

    void help() {
        // Only Prints Help Message
        printf(
            "Usage: %s <options> \n\n"
            "Options:"
            "\n\t-t,  --text:        \t<value>           \t:: input value to colorize"
            "\n\t-b,  --background:  \t<color>           \t:: output background color"
            "\n\t-f,  --foreground:  \t<color>           \t:: output foreground color"
            "\n\t-s,  --style:       \t<style>           \t:: output style"
            "\n\t-fm, --format:      \t<code>            \t:: output translated input <text>"
            "\n\t-l,  --list:        \t<true if present> \t:: list all possible colors/styles"
            "\n\t-nr, --no_reset:    \t<true if present> \t:: disables color reset at exit"
            "\n\t-nl, --no_newline:  \t<true if present> \t:: disables newline at exit"
            "\n\t-h,  --help:        \t<code>            \t:: prints help message"
            "\n",
            colormerge::generate({Color::Fore::BLUE, Color::Style::BRIGHT}, APPNAME, Color::PALETTE).c_str()
        );
    }
    void version() {
        printf("%s %.1f \n",
               colormerge::generate({Color::Fore::BLUE, Color::Style::BRIGHT}, APPNAME, Color::PALETTE).c_str(),
               APPVERSION
               );
    }
};

int main(int argc, char ** argv) {
    App app;
    return app.Run(argc, argv);
    return 0;
}
