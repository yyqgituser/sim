#include <vector>
#include <stdexcept>

#include "SimParser.h"
#include "SimSemanticor.h"
#include "SimParseAction.h"
#include "MemoryInputStream.h"
#include "SimParseException.h"

namespace parser {
  
void SimParser::parse() {
  int top = SimParserData::INIT_STATE;
  states.push_back(top);

  SimToken *current = bscanner->look(0);
  bool go = true;
  while (go) {
    if(top == SimParserData::ACCEPT_STATE) {
      break;
    }
    SimParseAction *action = SimParserData::ACTIONS[SimParserData::DFA_STATE_ACTIONS[top]];
    if (action->isLook()) {
      action = SimParserData::parseNext(action->value, current->id);
      if (action == 0) {
        throw SimParseException("Unexpected input", current->line, current->column);
      }
      int depth = 1;
      while (action->isLook()) {
        action = SimParserData::parseNext(action->value, bscanner->look(depth++)->id);
        if (action == 0) {
          throw SimParseException("Unexpected input", current->line, current->column);
        }
      }
    }
    switch (action->getType()) {
      case SimParseAction::ACTION_SHIFT: {
        int value = top = action->value;
        states.push_back(value);
        semanticor->outToken(current);
        bscanner->consume(1);
        current = bscanner->look(0);
        break;
      }
      case SimParseAction::ACTION_REDUCE: {
        int value = action->value;
        unsigned int *info = SimParserData::PROD_INFOS[value];
        unsigned int lhs = info[0];
        unsigned int rhsLen = info[1];
        states.erase(states.end() - rhsLen, states.end());
        top = states.back();
        top = SimParserData::gotoNext(top, lhs);
        states.push_back(top);
        break;
      }
      case SimParseAction::ACTION_ACTION: {
        int value = action->value;
        go = SimParserData::doAction(semanticor, value);
        top = SimParserData::gotoNext(top, value);
        states.push_back(top);
        break;
      }
    }
  }
}

}

