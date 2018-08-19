#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include "TextQueryComplicated.h"

TextQuery::TextQuery(std::ifstream &in) : file(new std::vector<std::string>)
{
    std::string text;
    while (getline(in, text)) {
        file->push_back(text);
        int n = file->size() - 1;
        std::istringstream line(text);
        std::string word;
        while (line >> word) {
            auto &lines = wm[word];
            if (!lines) {
                lines.reset(new std::set<line_no>);
            }
            lines->insert(n);
        }
    }
}

QueryResult
TextQuery::query(const std::string &word) const
{
    static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>{});
    auto loc = wm.find(word);
    if (loc == wm.end()) {
        return QueryResult(word, nodata, file);
    } else
        return QueryResult(word, loc->second, file);
}

std::string
make_plural(size_t ctr, const std::string &word, const std::string &ending)
{
    return (ctr<=1)?word:word+ending;
}

std::ostream& print(std::ostream &os, const QueryResult &qr)
{
    os << qr.sought << " occurs " << qr.lines->size() << " "
        << make_plural(qr.lines->size(), "time", "s") << std::endl;
    for (auto num : *qr.lines) {
        os << "\t(line " << num + 1 << ") "
            << *(qr.file->begin() + num) << std::endl;
    }
    return os;
}

QueryResult
NotQuery::eval(const TextQuery &text) const
{
    QueryResult result = query.eval(text);
    std::shared_ptr<std::set<line_no>> ret_lines = std::make_shared<std::set<line_no>>();
    std::size_t size = result.get_file()->size();
    auto beg = result.begin(), end = result.end();
    for (std::size_t n=0; n != size; n++)
    {
        if (beg == end || n != *beg) {
            ret_lines->insert(n);
        } else if (beg != end) {
            beg++;
        }
    }
    return QueryResult(rep(), ret_lines, result.get_file());
}

QueryResult
AndQuery::eval(const TextQuery &text) const
{
    auto left = lhs.eval(text), right = rhs.eval(text);
    auto ret_lines = std::make_shared<std::set<line_no>>();
    std::set_intersection(left.begin(), left.end(),
                          right.begin(), right.end(),
                          std::inserter(*ret_lines, ret_lines->begin()));
    return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult
OrQuery::eval(const TextQuery &text) const
{
    auto left = lhs.eval(text), right = rhs.eval(text);
    auto ret_lines = std::make_shared<std::set<line_no>>(left.begin(), right.end());
    ret_lines->insert(right.begin(), right.end());
    return QueryResult(rep(), ret_lines, left.get_file());
}
