#include "../src/unicode.h"

#include <cstdio>
#include <string>
#include <vector>

int main() {
    const struct {
        std::string text;
        std::vector<uint32_t> expected;
    } utf8_tests[] = {
        { "\x7f",                 { 0x7f } },
        { "\xc2\x80",             { 0x80 } },
        { "\xdf\xbf",             { 0x7ff } },
        { "\xe0\xa0\x80",         { 0x800 } },
        { "\xef\xbf\xbf",         { 0xffff } },
        { "\xf0\x90\x80\x80",     { 0x10000 } },
        { "\xf4\x8f\xbf\xbf",     { 0x10ffff } },
        { "\xc0\xa0",             { 0xfffd, 0xfffd } },
        { "\xc1\xbf",             { 0xfffd, 0xfffd } },
        { "\xe0\x80\xa0",         { 0xfffd, 0xfffd, 0xfffd } },
        { "\xe0\x9f\xbf",         { 0xfffd, 0xfffd, 0xfffd } },
        { "\xf0\x80\x80\xa0",     { 0xfffd, 0xfffd, 0xfffd, 0xfffd } },
        { "\xf0\x8f\xbf\xbf",     { 0xfffd, 0xfffd, 0xfffd, 0xfffd } },
        { "\xf4\x90\x80\x80",     { 0xfffd, 0xfffd, 0xfffd, 0xfffd } },
        { "\xf5\x80\x80\x80",     { 0xfffd, 0xfffd, 0xfffd, 0xfffd } },
        { "\xf7\xbf\xbf\xbf",     { 0xfffd, 0xfffd, 0xfffd, 0xfffd } },
        { "\xe2\x80",             { 0xfffd, 0xfffd } },
    };

    bool success = true;
    for (const auto & test : utf8_tests) {
        auto expected = test.expected;
        expected.insert(expected.begin(), 'a');
        expected.push_back('b');
        if (unicode_cpts_from_utf8("a" + test.text + "b") != expected) {
            fprintf(stderr, "unexpected UTF-8 decoding for bytes:");
            for (const unsigned char byte : test.text) {
                fprintf(stderr, " %02x", byte);
            }
            fprintf(stderr, "\n");
            success = false;
        }
    }

    const std::vector<std::string> regex_exprs = {
        "[~][A-Za-z]+| ?[\\p{S}]+|\\s+",
    };
    const std::vector<std::string> expected = { " ~", "foo" };
    const auto actual = unicode_regex_split(" ~foo", regex_exprs, false);

    if (actual != expected) {
        fprintf(stderr, "unexpected split:");
        for (const auto & piece : actual) {
            fprintf(stderr, " [%s]", piece.c_str());
        }
        fprintf(stderr, "\n");
        return 1;
    }

    return success ? 0 : 1;
}
