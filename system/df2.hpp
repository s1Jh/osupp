#pragma once

#include <string>
#include <Vec2.hpp>
#include <map>
#include <unordered_map>
#include <variant>

namespace GAME_TITLE {

    class df2 {
    public:

        df2();

        static df2 read(const std::string &path);

        void reload();

        static bool write(df2 &def, const std::string &path);

        enum class EntryType {
            String, Real, Integer,
            Vector, Boolean, Clump
        };

        std::string &str(const std::string & = "null");

        std::string &name(const std::string & = "null");

        double &real(const double = 0.0);

        int &integer(const int = 0);

        fvec2d &vec(const fvec2d & = {0.0, 0.0});

        bool &boolean(const bool = false);

        const std::string &str(const std::string & = "null") const;

        const std::string &name(const std::string & = "null") const;

        const double &real(const double & = 0.0) const;

        const int &integer(const int & = 0) const;

        const fvec2d &vec(const fvec2d & = {0.0, 0.0}) const;

        const bool &boolean(const bool & = false) const;

        EntryType getType() const;

        // expose some of the underlying functions of std::map
        df2 &operator[](const std::string &key);

        const df2 &operator[](const std::string &key) const;

        df2 &get(const std::string &key);

        const df2 &get(const std::string &key) const;

        std::map<const std::string, df2>::iterator find(const std::string &key);

        std::map<const std::string, df2>::const_iterator find(const std::string &key) const;

        std::map<const std::string, df2>::iterator begin();

        std::map<const std::string, df2>::iterator end();

        std::map<const std::string, df2>::reverse_iterator rbegin();

        std::map<const std::string, df2>::reverse_iterator rend();

        std::map<const std::string, df2>::const_iterator begin() const;

        std::map<const std::string, df2>::const_iterator end() const;

        std::map<const std::string, df2>::const_reverse_iterator rbegin() const;

        std::map<const std::string, df2>::const_reverse_iterator rend() const;

        size_t getSize() const;

        bool isEmpty() const;

        static void enableLogging();

        static void disableLogging();

        static void addAlias(std::string target, std::string replacement);

        static void removeAlias(std::string target);

        static df2 null;

    private:
        enum class SearchDirection {
            Forwards, Backwards
        };

        static std::string getToken(const std::string &, size_t start, SearchDirection dir = SearchDirection::Forwards);

        static void getClump(const std::string &chunk, df2 &clump, int end = -1, int resume = 0, int debug_level = 0);

        static void writeClump(std::stringstream &accum, df2 &clump, size_t level = 0);

        static int getEndOfClump(const std::string &str, char left, char right, int start);

        static bool enableSpam;
        static std::unordered_map<std::string, std::string> aliases;

        using DataType = std::variant<std::string, double, int, fvec2d, bool>;

        DataType data;
        std::string path;
        EntryType type;
        std::map<const std::string, df2> entries;
    };

}