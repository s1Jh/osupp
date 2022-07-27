#include "df2.hpp"

#include <cmath>
#include <fstream>
#include <sstream>
#include <type_traits>
#include <vector>

#include "Util.hpp"

NS_BEGIN

bool df2::enableSpam;

std::unordered_map<std::string, std::string> df2::aliases;

df2 df2::null;

void df2::reload()
{ *this = read(path); }

df2::df2() noexcept
    : type(df2::EntryType::Clump)
{}

df2 df2::read(const std::string &path)
{
    std::ifstream ifs(path, std::ios::in | std::ios::ate);
    if (!ifs.is_open()) {
        // Check if it exists, if not, tell the user and return.
        log::info("Failed to open file: ", path);
        return null;
    }
    // get the file size.
    size_t file_size = (size_t) ifs.tellg();
    // Reset back to the start
    ifs.seekg(0, std::ios::beg);
    // Create a vector to read the data into.
    std::vector<char> byte_data(file_size);
    // Read call, we read the whole file at once.
    // (after this constructor returns it will get deleted)
    ifs.read(byte_data.data(), file_size);
    ifs.close();
    // Put that data into a string representation.
    std::string str_repr(byte_data.data(), file_size);
    // Replace all aliases with their counterparts.
    for (auto &alias: aliases) {
        size_t start_pos = 0;
        std::stringstream replss;
        replss << '%' << alias.first << '%';
        std::string repl = replss.str();

        while ((start_pos = str_repr.find(repl)) != std::string::npos) {
            str_repr.replace(start_pos, repl.length(), alias.second);
        }
    }
    // Start the recursive read.
    df2 root;
    getClump(str_repr, root);
    return root;
}

std::string df2::getToken(const std::string &string, size_t start,
                          df2::SearchDirection dir)
{
    int traversal_rate = dir == SearchDirection::Forwards ? 1 : -1;
    bool found_first = false;
    std::vector<char> accum;
    std::string filtered_chars = "\n\t\a\f\r\t\v";

    // Iterate from the starting character onwards to the end
    // or to the start depending on the search direction.
    //
    // This will find any string encapsulated by two " characters.
    // It does so by iterating over the string in a direction.
    //
    // Once it finds a " it sets a flag and resets the loop in order to
    // not append the first " character to the result. After it finds
    // the first " it continually appends any characters to an accumulator.
    // It keeps doing that until it finds a second " character at which point
    // the loop breaks and we have the full token in the accumulator.
    for (size_t i = start; (i > 0 && i < string.size()); i += traversal_rate) {
        if (string.at(i) == '\"')
            if (!found_first) {
                found_first = true;
                continue;
            }
            else {
                break;
            }
        if (found_first) {
            // Remove invalid characters
            if (filtered_chars.find(string.at(i)) == std::string::npos) {
                if (dir == SearchDirection::Forwards) {
                    accum.push_back(string.at(i));
                }
                else {
                    accum.insert(accum.begin(), string.at(i));
                }
            }
        }
    }

    return {accum.data(), accum.size()};
}

void df2::getClump(const std::string &chunk, df2 &parent, int end, int resume,
                   int debug_level)
{
    if (enableSpam)
        log::debug("[PARSE] Level ", debug_level, " starting at ", resume);

    if (end == -1) {
        end = (int) chunk.size();
    }

    for (int i = resume; i < end; i++) {
        switch (chunk.at(i)) {
            case '$': {
                std::string token_name = getToken(chunk, i, SearchDirection::Forwards);
                if (enableSpam)
                    log::debug("[PARSE] Character ", i, " level ", debug_level,
                               " found tag \"", token_name, '\"');
                parent[token_name].boolean() = true;
                break;
            }
            case '=': {
                std::string token_name, token_value;
                token_name = getToken(chunk, i, SearchDirection::Backwards);
                token_value = getToken(chunk, i, SearchDirection::Forwards);
                if (enableSpam)
                    log::debug("[PARSE] Character ", i, " level ", debug_level,
                               " found value \"", token_name, "\" with value \"",
                               token_value, '\"');

                // all vector types are separated with a ";"
                auto vec_delim = token_value.find(';');
                if (vec_delim != std::string::npos) {
                    if (enableSpam)
                        log::debug("[PARSE] Deduced key \"", token_name, "\" (\"",
                                   token_value, "\") to be a vector type ");

                    std::string x_part = token_value.substr(0, vec_delim);
                    std::string y_part = token_value.substr(vec_delim + 1);

                    float x = 0.0;
                    float y = 0.0;

                    try {
                        x = std::stof(x_part);
                    }
                    catch (std::invalid_argument &e) {
                        log::warning("[PARSE] Token \"", token_name,
                                     "\" has malformed vector x component: \"", x_part, "\"");
                    }
                    catch (std::out_of_range &e) {
                        log::warning("[PARSE] Token \"", token_name,
                                     "\" has x component which is out of range of a double "
                                     "precision floating point number");
                    }

                    try {
                        y = std::stof(y_part);
                    }
                    catch (std::invalid_argument &e) {
                        log::warning("[PARSE] Token \"", token_name,
                                     "\" has malformed vector y component: \"", y_part, "\"");
                    }
                    catch (std::out_of_range &e) {
                        log::warning("[PARSE] Token \"", token_name,
                                     "\" has y component which is out of range of a double "
                                     "precision floating point number");
                    }

                    parent[token_name].vec() = {x, y};
                    break;
                }

                // all color types are prefixed with a #r,g,b,a

                if (token_value.starts_with('#')) {
                    auto data = token_value.substr(1);
                    auto fields = GetCharacterSeparatedValues(data, ',');
                    color &slot = parent[token_name].col();

                    for (unsigned int c = 0; c < 4; c++) {
                        uint8_t iValue;
                        if (c >= fields.size()) {
                            iValue = 255;
                        }
                        else {
                            try {
                                iValue = std::stoi(fields[c]);
                            }
                            catch (std::invalid_argument &e) {
                                log::warning("[PARSE] Color value ", token_name, ", channel ", c, " has invalid form");
                                iValue = 255;
                            }
                            catch (std::out_of_range &e) {
                                log::warning("[PARSE] Color value ", token_name, ", channel ", c, " is out of range");
                                iValue = 255;
                            }
                        }

                        float value = float(iValue) / 255.0f;

                        switch (c) {
                            case 0:
                                slot.r = value;
                                break;
                            case 1:
                                slot.g = value;
                                break;
                            case 2:
                                slot.b = value;
                                break;
                            case 3:
                                slot.a = value;
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                }

                // try to convert to a numeric format
                try {
                    double result = std::stod(token_value);

                    double integral;

                    if (std::modf(result, &integral) == 0.0) {
                        if (enableSpam)
                            log::debug("[PARSE] Deduced key \"", token_name, "\" (\"",
                                       token_value, "\") to be a integer type ");

                        parent[token_name].integer() = (int) result;
                        break;
                    }
                    if (enableSpam)
                        log::debug("[PARSE] Deduced key \"", token_name, "\" (\"",
                                   token_value, "\") to be a floating point type ");

                    parent[token_name].real() = result;
                    break;
                }
                catch (std::invalid_argument &e) {
                    // unable to convert to a floating point number, probably a string
                }
                catch (std::out_of_range &e) {
                    log::warning("[PARSE] Numeric key \"", token_name,
                                 "\" has a value out of range of a double precision float");
                }
                if (enableSpam)
                    log::debug("[PARSE] Deduced key \"", token_name, "\" (\"", token_value,
                               "\") to be a string type ");
                parent[token_name].str() = token_value;
                break;
            }

            case '(': {
                int old_i = i;

                i = getEndOfClump(chunk, '(', ')', i);
                if (enableSpam)
                    log::debug("[PARSE] Character ", old_i, " level ", debug_level,
                               " found comment going to ", i);
                break;
            }
            case '{': {
                std::string token_name = getToken(chunk, i, SearchDirection::Backwards);
                if (enableSpam)
                    log::debug("[PARSE] Character ", i, " level ", debug_level,
                               " found clump \"", token_name, '\"');

                auto &next_clump = parent[token_name];
                next_clump.name() = token_name;

                int next_start = i;
                i = getEndOfClump(chunk, '{', '}', i);

                getClump(chunk, next_clump, i - 1, next_start + 1, debug_level + 1);
                if (enableSpam)
                    log::debug("[PARSE] Returned to ", debug_level, " resuming at ", i);
                break;
            }
        }
    }
}

bool df2::write(df2 &def, const std::string &path)
{
    std::stringstream char_repr;
    char_repr << "( This file was automatically written by the program )\n\n";

    writeClump(char_repr, def);

    std::string str_repr(char_repr.str());

    // replace all aliases back to their aliased form
    for (auto &alias: aliases) {
        size_t start_pos = 0;
        std::stringstream replss;
        replss << alias.second;
        std::string repl = replss.str();

        while ((start_pos = str_repr.find(repl)) != std::string::npos) {
            str_repr.replace(start_pos, repl.length(), '%' + alias.first + '%');
        }
    }

    std::ofstream ofs(path, std::ios::out | std::ios::ate);
    if (!ofs.is_open()) {
        // Check if it exists, if not, tell the user and return.
        log::info("Failed to write file: ", path);
        return false;
    }

    ofs.write(str_repr.c_str(), str_repr.size());

    ofs.close();
    return true;
}

void df2::writeClump(std::stringstream &accum, df2 &clump, size_t level)
{
    for (auto &entry: clump) {
        for (size_t i = 0; i < level; i++)
            accum << '\t';

        switch (entry.second.getType()) {
            // "name" = "numvalue"
            case df2::EntryType::Integer: {
                int val = entry.second.integer();
                accum << '"' << entry.first << "\" = \"" << std::to_string(val) << '"';
                break;
            }
            case df2::EntryType::Real: {
                double val = entry.second.real();
                accum << '"' << entry.first << "\" = \"" << std::to_string(val) << '"';
                break;
            }
                // "name" = "strvalue"
            case df2::EntryType::String:
                accum << '"' << entry.first << "\" = \"" << entry.second.str() << '"';
                break;
                // $"<name>"
            case df2::EntryType::Boolean:
                accum << "$\"" << entry.first << '"';
                break;
                // "name" = "#R,G,B,A"
            case df2::EntryType::Color: {
                auto color = (color8) entry.second.col();
                accum << '"' << entry.first << "\" = \"#" << color.r << ',' << color.g << ','
                      << color.b << ',' << color.a << '"';
                break;
            }
                // "name" = { ... }
            case df2::EntryType::Clump:
                accum << '"' << entry.first << "\"\n";
                for (size_t i = 0; i < level; i++)
                    accum << '\t';
                accum << "{\n";

                writeClump(accum, entry.second, level + 1);

                for (size_t i = 0; i < level; i++)
                    accum << '\t';

                accum << "}";
                break;

                // "name" = "x;y"
            case df2::EntryType::Vector:
                accum << '"' << entry.first << "\" = \""
                      << "\"" << entry.second.vec().x << ";" << entry.second.vec().y
                      << "\"\n";
                break;
            default:
                break;
        }

        accum << '\n';
    }
}

int df2::getEndOfClump(const std::string &str, char left, char right,
                       int start)
{
    int rb = 0;
    int lb = 0;
    unsigned int i = start;
    for (; i < str.size(); i++) {
        char chr = str.at(i);

        if (chr == left)
            lb++;

        if (chr == right)
            rb++;

        if (lb == rb)
            break;
    }
    // we will always end up on the last } symbol
    if (lb != rb) {
        log::warning("Brace misscount detected in a df file: rb=", rb, "lb=", lb);
    }
    return i;
}

std::string &df2::str(const std::string &fallback)
{
    auto val = std::get_if<std::string>(&data);
    if (val && type == df2::EntryType::String) {
        return *val;
    }
    else {
        type = df2::EntryType::String;
        data = DataType(fallback);
        return std::get<std::string>(data);
    }
}

std::string &df2::name(const std::string &fallback)
{
    type = df2::EntryType::Clump;
    if (auto val = std::get_if<std::string>(&data)) {
        return *val;
    }
    else {
        data = DataType(fallback);
        return std::get<std::string>(data);
    }
}

double &df2::real(const double fallback)
{
    if (auto val = std::get_if<double>(&data)) {
        return *val;
    }
    else if (auto ival = std::get_if<int>(&data)) {
        return *((double *) ival);
    }
    else {
        type = df2::EntryType::Real;
        data = DataType(fallback);
        return std::get<double>(data);
    }
}

int &df2::integer(const int fallback)
{
    if (auto val = std::get_if<int>(&data)) {
        return *val;
    }
    else if (auto dval = std::get_if<int>(&data)) {
        return *((int *) dval);
    }
    else {
        type = df2::EntryType::Integer;
        data = DataType(fallback);
        return std::get<int>(data);
    }
}

fvec2d &df2::vec(const fvec2d &fallback)
{
    if (auto val = std::get_if<fvec2d>(&data)) {
        return *val;
    }
    else {
        type = df2::EntryType::Vector;
        data = DataType(fallback);
        return std::get<fvec2d>(data);
    }
}

bool &df2::boolean(const bool fallback)
{
    if (auto val = std::get_if<bool>(&data)) {
        return *val;
    }
    else {
        type = df2::EntryType::Boolean;
        data = DataType(fallback);
        return std::get<bool>(data);
    }
}

color &df2::col(const struct color &fallback)
{
    if (auto val = std::get_if<struct color>(&data)) {
        return *val;
    }
    else {
        type = df2::EntryType::Boolean;
        data = DataType(fallback);
        return std::get<struct color>(data);
    }
}

const std::string &df2::str(const std::string &fallback) const
{
    if (const auto val = std::get_if<std::string>(&data)) {
        return *val;
    }
    else {
        return fallback;
    }
}

const std::string &df2::name(const std::string &fallback) const
{
    if (auto val = std::get_if<std::string>(&data)) {
        return *val;
    }
    else {
        return {fallback};
    }
}

const double &df2::real(const double &fallback) const
{
    if (auto val = std::get_if<double>(&data)) {
        return *val;
    }
    else if (auto ival = std::get_if<int>(&data)) {
        return *((double *) ival);
    }
    else {
        return fallback;
    }
}

const int &df2::integer(const int &fallback) const
{
    if (auto val = std::get_if<int>(&data)) {
        return *val;
    }
    else if (auto dval = std::get_if<double>(&data)) {
        return *((int *) dval);
    }
    else {
        return fallback;
    }
}

const fvec2d &df2::vec(const fvec2d &fallback) const
{
    if (auto val = std::get_if<fvec2d>(&data)) {
        return *val;
    }
    else {
        return fallback;
    }
}

const bool &df2::boolean(const bool &fallback) const
{
    if (auto val = std::get_if<bool>(&data)) {
        return *val;
    }
    else {
        return fallback;
    }
}

const color &df2::col(const struct color &fallback) const
{
    if (auto val = std::get_if<struct color>(&data)) {
        return *val;
    }
    else {
        return fallback;
    }
}

df2::EntryType df2::getType() const
{ return type; }

df2 &df2::operator[](const std::string &key)
{ return entries[key]; }

const df2 &df2::operator[](const std::string &key) const
{
    if (!isEmpty())
        if (entries.find(key) != entries.cend())
            return entries.at(key);

    return null;
}

df2 &df2::get(const std::string &key)
{ return entries[key]; }

const df2 &df2::get(const std::string &key) const
{
    if (!isEmpty())
        if (entries.find(key) != entries.cend())
            return entries.at(key);

    return null;
}

std::map<const std::string, df2>::iterator df2::find(const std::string &key)
{
    return entries.find(key);
}

std::map<const std::string, df2>::const_iterator
df2::find(const std::string &key) const
{
    return entries.find(key);
}

std::map<const std::string, df2>::iterator df2::begin()
{
    return entries.begin();
}

std::map<const std::string, df2>::iterator df2::end()
{ return entries.end(); }

std::map<const std::string, df2>::reverse_iterator df2::rbegin()
{
    return entries.rbegin();
}

std::map<const std::string, df2>::reverse_iterator df2::rend()
{
    return entries.rend();
}

std::map<const std::string, df2>::const_iterator df2::begin() const
{
    return entries.begin();
}

std::map<const std::string, df2>::const_iterator df2::end() const
{
    return entries.end();
}

std::map<const std::string, df2>::const_reverse_iterator df2::rbegin() const
{
    return entries.rbegin();
}

std::map<const std::string, df2>::const_reverse_iterator df2::rend() const
{
    return entries.rend();
}

size_t df2::getSize() const
{ return entries.size(); }

bool df2::isEmpty() const
{ return entries.empty(); }

void df2::enableLogging()
{ enableSpam = true; }

void df2::disableLogging()
{ enableSpam = false; }

void df2::addAlias(std::string target, std::string replacement)
{
    // Log::info("Added default alias ", target, "\t\t| ", replacement);
    aliases[target] = replacement;
}

void df2::removeAlias(std::string target)
{
    for (auto it = aliases.begin(); it != aliases.end();) {
        if (it->first == target) {
            it = aliases.erase(it);
            return;
        }
        else {
            it++;
        }
    }
}

NS_END