#include <algorithm>

#include <AQSyntaxHighlighter.h>
#include <AQTextDoc.h>

#include <string.h>
#include <ctype.h>
#include <stdio.h>

char *keywords[] = {"alignas"
 , "alignof"
 , "and"
 , "and_eq"
 , "asm"
 , "atomic_cancel"
 , "atomic_commit"
 , "atomic_noexcept"
 , "auto"
 , "bitand"
 , "bitor"
 , "bool"
 , "break"
 , "case"
 , "catch"
 , "char"
 , "char16_t"
 , "char32_t"
 , "class"
 , "compl"
 , "concept"
 , "const"
 , "constexpr"
 , "const_cast"
 , "continue"
 , "co_await"
 , "co_return"
 , "co_yield"
 , "decltype"
 , "default"
 , "delete"
 , "do"
 , "double"
 , "dynamic_cast"
 , "else"
 , "enum"
 , "explicit"
 , "export"
 , "extern"
 , "false"
 , "float"
 , "for"
 , "friend"
 , "goto"
 , "if"
 , "import"
 , "inline"
 , "int"
 , "long"
 , "module"
 , "mutable"
 , "namespace"
 , "new"
 , "noexcept"
 , "not"
 , "not_eq"
 , "nullptr"
 , "operator"
 , "or"
 , "or_eq"
 , "private"
 , "protected"
 , "public"
 , "register"
 , "reflexpr"
 , "reinterpret_cast"
 , "requires"
 , "return"
 , "short"
 , "signed"
 , "sizeof"
 , "static"
 , "static_assert"
 , "static_cast"
 , "struct"
 , "switch"
 , "synchronized"
 , "template"
 , "this"
 , "thread_local"
 , "throw"
 , "true"
 , "try"
 , "typedef"
 , "typeid"
 , "typename"
 , "union"
 , "unsigned"
 , "using"
 , "virtual"
 , "void"
 , "volatile"
 , "wchar_t"
 , "while"
 , "xor"
 , "xor_eq"
 , "#include"
 , "#if"
 , "#ifdef"
 , "#ifndef"
 , "#endif"
 , "#else"
 , "#define"
 , nullptr
};

struct CharState
{
   CharState() : symbol(0), terminal(false), repeating(false), counts(true) {}

   bool charMatch(char c) {
      if (symbol==1) // Wildcard
         return true;
      if (symbol==2) { // NonWord Wildcard
         return (c != '_' && !isalnum(c));
      }
      if (symbol == c)
         return true;
      return false;
   }
   char symbol;
   vector<CharState *> nexts;
   int color;
   bool terminal;
   bool repeating;
   bool counts;
};

AQSyntaxHighlighter::AQSyntaxHighlighter(AQTextDoc *doc)
   : m_blockNumber(0)
   , m_doc(doc)
   , m_nonWordState(new CharState)
   , m_defaultState(new CharState)
{
   doc->m_highlighter = this;

   m_nonWordState->symbol = 2; // nonword wildcard
   m_defaultState->repeating = true;
   m_defaultState->symbol = 1; // always matches wildcard

   char cppcomment[] = {'/','/', 1, 0};
   addKeyWord(cppcomment, 5);
   char ccommentstart[] = {'/', '*', 1, 0};
   addKeyWord(cppcomment, 5);

   char strKeyword[] = {'"',1,'"', 0, true};
   addKeyWord(strKeyword, 4);
   char charlitescKeyword[] = {'\'','\\', 1, '\'', 0};
   addKeyWord(charlitescKeyword, 7, true);
   char charlitKeyword[] = {'\'', 1, '\'', 0};
   addKeyWord(charlitKeyword, 7, true);

   int i = 0;
   while (keywords[i])
      addKeyWord(keywords[i++], 6, true);

   m_defaultState->nexts.push_back(m_nonWordState);
}

void AQSyntaxHighlighter::highlightBlock(const AQString &text)
{
   int cpos = 0;
   int matchLength = 0;
   int matchStart = 0;
   CharState *state = m_nonWordState;
   while (cpos < text.size()) {
      char symbol = text[cpos];
      int i = 0;
      while (i < state->nexts.size()) {
         if (state->nexts[i]->charMatch(symbol))
            break;

         ++i;
      }
      if (i == state->nexts.size()) {
         // match failed
         if (state->repeating && state->charMatch(symbol)) {
            ++matchLength;
            ++cpos;
            continue;
         }
         if (state->terminal)
            setFormat(matchStart, matchLength, state->color);
         if (!state->counts)
            --cpos;
         state = m_defaultState;
         matchLength = 0;
      } else {
         if (state == m_defaultState || state == m_nonWordState) {
            matchStart = cpos;
            matchLength = 0;
         }
         state = state->nexts[i];
         if (state->counts)
            ++matchLength;
         ++cpos;
      }
   }
   if (state && state->terminal)
      setFormat(matchStart, matchLength, state->color);   
}

void AQSyntaxHighlighter::setBlockNumber(int b)
{
   m_blockNumber = b;
}

void AQSyntaxHighlighter::addKeyWord(char *keyword, int color, int boundNonWord)
{
   char *ptr = keyword;
   CharState *state = boundNonWord ? m_nonWordState : m_defaultState;
   while (*ptr) {
      int i = 0;
      while (i < state->nexts.size()) {
         if (state->nexts[i]->symbol == *ptr)
            break;
         ++i;
      }
      if (i == state->nexts.size())
         state->nexts.push_back(new CharState);

      state = state->nexts[i];
      state->symbol = *ptr;
      state->repeating = *ptr == 1;
      ++ptr;
   }
   if (boundNonWord) {
      state->nexts.push_back(new CharState);
      state = state->nexts.back();
      state->symbol = 2;
      state->counts = false;
   }
   state->color = color;
   state->terminal = true;
}

void AQSyntaxHighlighter::setFormat(int start, int count, int color)
{
   if (m_doc->m_highlightRanges[m_blockNumber] == nullptr) {
      AQFormatRange *rng = new AQFormatRange(color, -1, nullptr);
      rng->setStartEnd(start, start+count);
      
      m_doc->m_highlightRanges[m_blockNumber] = rng;
   } else {
      AQFormatRange rng(color, -1, nullptr);
      rng.setStartEnd(start, start+count);
      
      m_doc->m_highlightRanges[m_blockNumber]->modifyWith(&rng);
   }
}


