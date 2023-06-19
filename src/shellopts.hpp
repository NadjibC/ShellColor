#ifndef SHELLOPTS_H_
#define SHELLOPTS_H_ 0.9 // Version

#include <iostream>
#include <unordered_map>
#include <vector>

struct Option {
  unsigned int type; // An Option Types Could Be: 0 For String Value | 1 For Boolean Value
  std::string long_flag; // long flag is long form of a regular flag which starts with "--" (double dashes)
  std::string short_flag; // short flag is short form of a regular flag which starts with only "-" (single dash)
  std::string help_line; // stores help sequences, which are used to generate a help message (via ShellOpts::generate_help)
  std::string content; // stores a specific argument/option content, which can be a "true" or "false" if Option::type is 1 for Boolean, or a string which has been given from the next "argv" argument.
};

class ShellOpts {
public:
  std::unordered_map<std::string, Option> options; // options.first is for the general names of an option, option.second stores An Option struct
  std::vector<std::string> unregistered_options; // unregistered_options: stores string values of unregistered shell arguments/options which has been given by the user
  std::vector<std::string> help_types = {"<value>", /*"[BOOL]"*/ ""}; // Index 0: For String Value Type; Index 1: For Boolean Type

  std::string get_flag_name(std::string flag) {
    /*
    get_flag_name: returns the name of the flag of an option struct.

    Arguments:
      flag: string value used to search for its option struct name.
    Returns: string value of an name if found.
    */
    flag = remove_dashes(flag); // Remove flag dashes because flags aren't stored with dashes
    for (const auto & [name, option] : options) {
      if (option.long_flag == flag || option.short_flag == flag) return name;
    }
    throw 1; // throw 1 if the flag's name is not found.
  }

  Option& get_option_by_name(std::string name) {
    if (options.find(name) != options.end())
      return options[name];
    else throw 2; // throw 2 if name is not found in options as a key
  }
  Option& get_option_by_flag(std::string flag)
    { return options[get_flag_name(flag)]; }

  std::string get_content(std::string name)
    { return get_option_by_name(name).content; }

  Option make_option(const unsigned int &type, const std::string &long_flag, const std::string &short_flag="", const std::string &help_line="", const std::string &default_content = "") {
    Option option;
    option.type = type;
    option.long_flag = long_flag;
    option.short_flag = short_flag;
    option.help_line = help_line;
    if (!default_content.empty()) option.content = default_content;
    else if (type == 1) option.content = "false";
    return option;
  }

  bool is_registered_flag(std::string flag) {
    for (const auto option : options) {
      if ((option.second.short_flag == remove_dashes(flag) && (is_flag("-"+flag.substr(1, flag.size()-1)) == 1)) || ((option.second.long_flag == remove_dashes(flag)) && (is_flag(flag) == 2))) return true;
    }
    return false;
  }
  std::string generate_help(const std::string& header, const int& spacing=5, const std::string& flags_sep = ", --") {
    // generate_help: generates a help message (self-explanatory).
    std::string help = header + "\n\n";
    int highest_long_flag_size = 0, highest_help_type_size = 0;
    for (auto const &[name, option] : options) {
      if (option.long_flag.size() > highest_long_flag_size) highest_long_flag_size = option.long_flag.size();
      if (help_types[option.type].size() > highest_help_type_size) highest_help_type_size = help_types[option.type].size();
    }
    for (const auto &[name, option] : options)
      help += "\t-" + option.short_flag + flags_sep + option.long_flag + repeat_c(' ', highest_long_flag_size-option.long_flag.size()+2) +  help_types[option.type] + repeat_c(' ', highest_help_type_size-help_types[option.type].size()+spacing) + option.help_line + "\n";

    return help;
  }

  void print_help(std::string header) {
    // print_help: Generates A Help Message and writes to the output buffer (self-explanatory).
    std::cout << generate_help(header) << std::endl;
  }

  std::string repeat_c(const char &c, int times) {
    // repeat_c: Generate a string list of c repeated by "times" (self-explanatory).
    std::string output;
    for (int x=0; x<times; x++) output += c;
    return output;
  }

  inline void add_option(const std::string &name, Option option) 
    { 
      for (const auto & [id, op] : options)
        if (op.type == option.type && (op.long_flag == option.long_flag || op.short_flag == option.short_flag)
            && op.content == option.content || name == id) throw 1;
      options.emplace(name, option); 
    }

  std::string remove_dashes(std::string flag) {
    switch (is_flag(flag)) {
      case 2:
       return flag.substr(2, flag.size()-2);
      case 1: return flag.substr(1, flag.size()-1);
    }
    return flag;
  }

  int is_flag(std::string ambiguous_flag) {
    if (ambiguous_flag[0] == '-' && ambiguous_flag[1] == '-') return 2;
    else if (ambiguous_flag[0] == '-') return 1;
    return 0;
  }

void parse_args(int argc, char ** argv) {
  for (int x=0; x<argc; x++) {
    int flag_type = is_flag(argv[x]);
    switch (is_flag(argv[x])) {
      case 2:
        if (is_registered_flag(argv[x])) {
          Option& option = get_option_by_flag(argv[x]);
          if (option.type == 0) option.content = argv[++x];
          else if (option.type == 1) option.content = "true";
        }
        else unregistered_options.emplace_back(argv[x]);
        break;
      case 1:
        for (char character : remove_dashes(std::string(argv[x]))) {
          if (is_registered_flag(std::string(1, character))) {
            Option& option = get_option_by_flag(std::string(1, character));
            if (option.type == 0) option.content = argv[++x];
            else if (option.type == 1) option.content = "true";
          }
          else unregistered_options.emplace_back(std::string(1, character));
        }
        break;
      }
    }
  }
};

#endif // SHELLOPTS_H_-
