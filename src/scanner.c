#include "stdio.h"
#include "stdlib.h"
#include <tree_sitter/parser.h>

enum TokenType { NO, INV };

struct state {
  bool maybe_no;
};

void *tree_sitter_vim_external_scanner_create() { return NULL; }

void tree_sitter_vim_external_scanner_destroy(void *payload) {}

unsigned int tree_sitter_vim_external_scanner_serialize(void *payload,
                                                        char *buffer) {
  return 0;
}

void tree_sitter_vim_external_scanner_deserialize(void *payload,
                                                  const char *buffer,
                                                  unsigned length) {}

static void advance(TSLexer *lexer, bool skip) { lexer->advance(lexer, skip); }

void skip_space_tabs(TSLexer *lexer) {
  while (lexer->lookahead == ' ' || lexer->lookahead == '\t') {
    advance(lexer, true);
  }
}

bool check_prefix(TSLexer *lexer, char *preffix, unsigned int preffix_len,
                  enum TokenType token) {
  for (unsigned int i = 0; i < preffix_len; i++) {
    if (lexer->lookahead == preffix[i]) {
      advance(lexer, false);
    } else {
      return false;
    }
  }

  lexer->result_symbol = token;
  return true;
}

bool tree_sitter_vim_external_scanner_scan(void *payload, TSLexer *lexer,
                                           const bool *valid_symbols) {
  (void)payload;

  skip_space_tabs(lexer);

  if (valid_symbols[NO] && lexer->lookahead == 'n') {
    // options can be inverted by prepending a 'no' or 'inv'
    return check_prefix(lexer, "no", 2, NO);
  } else if (valid_symbols[INV] && lexer->lookahead == 'i') {
    return check_prefix(lexer, "inv", 3, INV);
  }

  return false;
}

// vim: tabstop=2
