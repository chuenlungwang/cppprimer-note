#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <set>
#include <map>
#include <vector>
#include "TextQuery.h"

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
