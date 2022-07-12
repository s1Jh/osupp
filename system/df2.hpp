#pragma once

#include <Vec2.hpp>
#include <map>
#include <string>
#include <unordered_map>
#include <variant>
#include "Color.hpp"

NS_BEGIN

class df2
{
public:
    df2();

    static df2 read(const std::string &path);

    void reload();

    static bool write(df2 &def, const std::string &path);

    enum class EntryType
    {
        String, Real, Integer, Vector, Boolean, Color, Clump
    };

    std::string &str(const std::string & = "null");

    std::string &name(const std::string & = "null");

    double &real(double = 0.0);

    int &integer(int = 0);

    fvec2d &vec(const fvec2d & = {0.0, 0.0});

    bool &boolean(bool = false);

    color &col(const color & = WHITE);

    [[nodiscard]] const std::string &str(const std::string & = "null") const;

    [[nodiscard]] const std::string &name(const std::string & = "null") const;

    [[nodiscard]] const double &real(const double & = 0.0) const;

    [[nodiscard]] const int &integer(const int & = 0) const;

    [[nodiscard]] const fvec2d &vec(const fvec2d & = {0.0, 0.0}) const;

    [[nodiscard]] const bool &boolean(const bool & = false) const;

    [[nodiscard]] const color &col(const color & = WHITE) const;

    [[nodiscard]] EntryType getType() const;

    // expose some of the underlying functions of std::map
    df2 &operator[](const std::string &key);

    const df2 &operator[](const std::string &key) const;

    df2 &get(const std::string &key);

    [[nodiscard]] const df2 &get(const std::string &key) const;

    [[nodiscard]] std::map<const std::string, df2>::iterator find(const std::string &key);

    [[nodiscard]] std::map<const std::string, df2>::const_iterator find(const std::string &key) const;

    [[nodiscard]] std::map<const std::string, df2>::iterator begin();

    [[nodiscard]] std::map<const std::string, df2>::iterator end();

    [[nodiscard]] std::map<const std::string, df2>::reverse_iterator rbegin();

    [[nodiscard]] std::map<const std::string, df2>::reverse_iterator rend();

    [[nodiscard]] std::map<const std::string, df2>::const_iterator begin() const;

    [[nodiscard]] std::map<const std::string, df2>::const_iterator end() const;

    [[nodiscard]] std::map<const std::string, df2>::const_reverse_iterator rbegin() const;

    [[nodiscard]] std::map<const std::string, df2>::const_reverse_iterator rend() const;

    [[nodiscard]] size_t getSize() const;

    [[nodiscard]] bool isEmpty() const;

    static void enableLogging();

    static void disableLogging();

    static void addAlias(std::string target, std::string replacement);

    static void removeAlias(std::string target);

    static df2 null;

private:
    enum class SearchDirection
    {
        Forwards, Backwards
    };

    static std::string getToken(const std::string &, size_t start,
                                SearchDirection dir = SearchDirection::Forwards);

    static void getClump(const std::string &chunk, df2 &clump, int end = -1,
                         int resume = 0, int debug_level = 0);

    static void writeClump(std::stringstream &accum, df2 &clump,
                           size_t level = 0);

    static int getEndOfClump(const std::string &str, char left, char right,
                             int start);

    static bool enableSpam;
    static std::unordered_map<std::string, std::string> aliases;

    using DataType = std::variant<std::string, double, int, fvec2d, bool, color>;

    DataType data;
    std::string path;
    EntryType type;
    std::map<const std::string, df2> entries;
};

NS_END