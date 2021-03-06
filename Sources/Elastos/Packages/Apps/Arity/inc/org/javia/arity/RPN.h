//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#ifndef _ORG_JAVIA_ARITY_RPN_H__
#define _ORG_JAVIA_ARITY_RPN_H__

#include "org/javia/arity/TokenConsumer.h"
#include "org/javia/arity/Token.h"

using Elastos::Utility::IStack;

namespace Org {
namespace Javia {
namespace Arity {

/* Reverse Polish Notation
   reads tokens in normal infix order (e.g.: 1 + 2)
   and outputs them in Reverse Polish order (e.g.: 1 2 +).
   See Dijkstra's Shunting Yard algorithm:
   http://en.wikipedia.org/wiki/Shunting_yard_algorithm
 */
class RPN : public TokenConsumer
{
public:
    RPN();

    CARAPI_(void) SetConsumer(
        /* [in] */ TokenConsumer* consumer);

    CARAPI_(void) Start();

    static CARAPI_(Boolean) IsOperand(
        /* [in] */ Int32 id);

    CARAPI Push(
        /* [in] */ Token* token);

private:
    CARAPI_(AutoPtr<Token>) Top();

    CARAPI PopHigher(
        /* [in] */ Int32 priority);

public:
    AutoPtr<IStack> mStack;
    Int32 mPrevTokenId;
    AutoPtr<TokenConsumer> mConsumer;
};

} // namespace Arity
} // namespace Javia
} // namespace Org

#endif // _ORG_JAVIA_ARITY_RPN_H__