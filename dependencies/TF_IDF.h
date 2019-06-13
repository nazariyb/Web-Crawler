//#ifndef TF_IDF
//#define TF_IDF
//
//#include <sstream>
//#include <vector>
//#include <map>
//#include <set>
//#include <cmath>
//#include <boost/tokenizer.hpp>
//
//#include "TF_IDF.h"
//#include "./reader/word_usings.h"
//
//using vectorString = std::vector<std::string>;
//
//struct TF_IDF
//    {
//    std::vector<std::pair<std::string, std::map<std::string, int>>> files_statistics;
//    std::map<std::string, std::vector<double>> files_frequencies;
//    vectorString unique_words_list;
//
//    TF_IDF (std::vector<std::pair<std::string, vectorString> > files_to_parse)
//    {
//        for (auto &file_to_parse: files_to_parse) {
//            parse_file(file_to_parse.first, file_to_parse.second);
//        }
//    };
//
//    void compute ()
//    {
//        create_unique_words_list();
//        for (auto &file_statistic: files_statistics) {
//            for (auto &word: unique_words_list) {
//                files_frequencies[file_statistic.first].push_back(tf_idf(word, file_statistic.second));
//            }
//        }
//    }
//
//    void create_unique_words_list ()
//    {
//        std::set<std::string> unique_words_set;
//        for (auto &document : files_statistics) {
//            for (auto &word : document.second)
//                unique_words_set.insert(word.first);
//        }
//        std::copy(unique_words_set.begin(), unique_words_set.end(), std::back_inserter(unique_words_list));
//    }
//
//    void parse_file (std::string filename, vectorString &file_to_parse)
//    {
//        std::map<std::string, int> file_statistics;
//        for (auto &string_in_file: file_to_parse) {
//            boost::tokenizer<> tok(string_in_file);
//            for (boost::tokenizer<>::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
//                ++file_statistics[*beg];
//            }
//        }
//        files_statistics.push_back(std::make_pair(filename, file_statistics));
//    }
//
//    unsigned count_terms_in_document (std::map<std::string, int> &file_statistic)
//    {
//        unsigned s{0};
//        for (auto &word: file_statistic) {
//            s += word.second;
//        }
//        return s;
//    }
//
//    double term_frequency (std::string term, std::map<std::string, int> &file_statistic)
//    {
//        int term_in_document = file_statistic[term];
//        unsigned len_of_document = count_terms_in_document(file_statistic);
//        double normalized_tf = static_cast<double>(term_in_document) / len_of_document;
//        return normalized_tf;
//    }
//
//    double inverse_document_frequency (std::string &term)
//    {
//        int docs_with_given_term{0};
//
//        for (auto &file_statistic: files_statistics) {
//            auto try_to_find = file_statistic.second.find(term);
//            if (try_to_find != file_statistic.second.end())
//                ++docs_with_given_term;
//        }
//
//        if (docs_with_given_term > 0) {
//            int docs_num = files_statistics.size();
//            return (log(static_cast<double>(docs_num) / docs_with_given_term));
//        } else {
//            return 0;
//        }
//    }
//
//    double tf_idf (std::string term, std::map<std::string, int> &file_statistic)
//    {
//        return term_frequency(term, file_statistic) * inverse_document_frequency(term);
//    }
//
//    double dot_product (const std::pair<std::string, std::vector<double>> &a,
//                        const std::pair<std::string, std::vector<double>> &b)
//    {
//        double res = 0;
//        for (int i = 0; i < a.second.size(); ++i) {
//            res += a.second[i] * b.second[i];
//        }
//        return res;
//    }
//
//    double vector_length (const std::pair<std::string, std::vector<double>> &a)
//    {
//        return sqrt(dot_product(a, a));
//    }
//
//    double cosine_similarity (const std::pair<std::string, std::vector<double>> &a,
//                              const std::pair<std::string, std::vector<double>> &b)
//    {
//        return dot_product(a, b) / (vector_length(a) * vector_length(b));
//    }
//    };
//
//
//#endif //TF_IDF
