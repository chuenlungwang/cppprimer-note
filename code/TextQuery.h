#ifndef __TEXT_QUERY_H__
#define __TEXT_QUERY_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <set>
#include <map>
#include <vector>

class QueryResult;

class TextQuery {
public:
    using line_no = std::vector<std::string>::size_type;
    explicit TextQuery(std::ifstream &);
public:
    QueryResult query(const std::string &) const;
private:
    std::shared_ptr<std::vector<std::string>> file;
    std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};

class QueryResult {
friend std::ostream& print(std::ostream&, const QueryResult&);
public:
    QueryResult(std::string s,
                std::shared_ptr<std::set<TextQuery::line_no>> p,
                std::shared_ptr<std::vector<std::string>> f)
        : sought(s),
          lines(p),
          file(f)
    {}
private:
    std::string sought;
    std::shared_ptr<std::set<TextQuery::line_no>> lines;
    std::shared_ptr<std::vector<std::string>> file;
};

#endif
