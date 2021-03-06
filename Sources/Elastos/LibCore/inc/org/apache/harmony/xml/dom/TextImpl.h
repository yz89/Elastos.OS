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

#ifndef __ORG_APACHE_HARMONY_XML_DOM_TEXTIMPL_H__
#define __ORG_APACHE_HARMONY_XML_DOM_TEXTIMPL_H__

#include "Elastos.CoreLibrary.External.h"
#include <elastos/core/Object.h>
#include "org/apache/harmony/xml/dom/CharacterDataImpl.h"

using Org::W3c::Dom::IDocument;
using Org::W3c::Dom::IText;
using Elastos::Core::Object;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xml {
namespace Dom {

class TextImpl
    : public CharacterDataImpl
    , public IText
{
public:
    CAR_INTERFACE_DECL();

    TextImpl();

    CARAPI constructor(
        /* [in] */ IDocument* document,
        /* [in] */ const String& data);

    CARAPI GetNodeName(
        /* [out] */ String * pStr);

    CARAPI GetNodeType(
        /* [out] */ Int16 * pValue);

    //CARAPI GetNodeValue(
    //    /* [out] */ String * pStr);

    //CARAPI SetNodeValue(
    //    /* [in] */ const String& nodeValue);

    //CARAPI GetParentNode(
    //    /* [out] */ Org::W3c::Dom::INode ** ppNode);

    //CARAPI GetChildNodes(
    //    /* [out] */ Org::W3c::Dom::INodeList ** ppNodelist);

    //CARAPI GetFirstChild(
    //    /* [out] */ Org::W3c::Dom::INode ** ppNode);

    //CARAPI GetLastChild(
    //    /* [out] */ Org::W3c::Dom::INode ** ppNode);

    //CARAPI GetPreviousSibling(
    //    /* [out] */ Org::W3c::Dom::INode ** ppNode);

    //CARAPI GetNextSibling(
    //    /* [out] */ Org::W3c::Dom::INode ** ppNode);

    //CARAPI GetAttributes(
    //    /* [out] */ Org::W3c::Dom::INamedNodeMap ** ppNodemap);

    //CARAPI GetOwnerDocument(
    //    /* [out] */ Org::W3c::Dom::IDocument ** ppDoc);

    //CARAPI InsertBefore(
    //    /* [in] */ Org::W3c::Dom::INode * pNewChild,
    //    /* [in] */ Org::W3c::Dom::INode * pRefChild,
    //    /* [out] */ Org::W3c::Dom::INode ** ppNode);

    //CARAPI ReplaceChild(
    //    /* [in] */ Org::W3c::Dom::INode * pNewChild,
    //    /* [in] */ Org::W3c::Dom::INode * pOldChild,
    //    /* [out] */ Org::W3c::Dom::INode ** ppNode);

    //CARAPI RemoveChild(
    //    /* [in] */ Org::W3c::Dom::INode * pOldChild,
    //    /* [out] */ Org::W3c::Dom::INode ** ppNode);

    //CARAPI AppendChild(
    //    /* [in] */ Org::W3c::Dom::INode * pNewChild,
    //    /* [out] */ Org::W3c::Dom::INode ** ppNode);

    //CARAPI HasChildNodes(
    //    /* [out] */ Boolean * pValue);

    //CARAPI CloneNode(
    //    /* [in] */ Boolean deep,
    //    /* [out] */ Org::W3c::Dom::INode ** ppNode);

    //CARAPI Normalize();

    //CARAPI IsSupported(
    //    /* [in] */ const String& feature,
    //    /* [in] */ const String& ver,
    //    /* [out] */ Boolean * pValue);

    //CARAPI GetNamespaceURI(
    //    /* [out] */ String * pStr);

    //CARAPI GetPrefix(
    //    /* [out] */ String * pStr);

    //CARAPI SetPrefix(
    //    /* [in] */ const String& prefix);

    //CARAPI GetLocalName(
    //    /* [out] */ String * pStr);

    //CARAPI HasAttributes(
    //    /* [out] */ Boolean * pValue);

    //CARAPI GetBaseURI(
    //    /* [out] */ String * pStr);

    //CARAPI CompareDocumentPosition(
    //    /* [in] */ Org::W3c::Dom::INode * pOther,
    //    /* [out] */ Int16 * pValue);

    //CARAPI GetTextContent(
    //    /* [out] */ String * pStr);

    //CARAPI SetTextContent(
    //    /* [in] */ const String& textContent);

    //CARAPI IsSameNode(
    //    /* [in] */ Org::W3c::Dom::INode * pOther,
    //    /* [out] */ Boolean * pValue);

    //CARAPI LookupPrefix(
    //    /* [in] */ const String& namespaceURI,
    //    /* [out] */ String * pStr);

    //CARAPI IsDefaultNamespace(
    //    /* [in] */ const String& namespaceURI,
    //    /* [out] */ Boolean * pValue);

    //CARAPI LookupNamespaceURI(
    //    /* [in] */ const String& prefix,
    //    /* [out] */ String * pStr);

    //CARAPI IsEqualNode(
    //    /* [in] */ Org::W3c::Dom::INode * pArg,
    //    /* [out] */ Boolean * pValue);

    //CARAPI GetFeature(
    //    /* [in] */ const String& feature,
    //    /* [in] */ const String& ver,
    //    /* [out] */ IObject ** ppObj);

    //CARAPI SetUserData(
    //    /* [in] */ const String& key,
    //    /* [in] */ IObject * pData,
    //    /* [in] */ Org::W3c::Dom::IUserDataHandler * pHandler,
    //    /* [out] */ IObject ** ppObj);

    //CARAPI GetUserData(
    //    /* [in] */ const String& key,
    //    /* [out] */ IObject ** ppObj);

    //CARAPI IsParentOf(
    //    /* [in] */ Org::W3c::Dom::INode * pNode,
    //    /* [out] */ Boolean * pIsParentOf);

    //CARAPI AppendDataTo(
    //    /* [in] */ Elastos::Core::IStringBuilder * pStringBuilder);

    //CARAPI GetData(
    //    /* [out] */ String * pStr);

    //CARAPI SetData(
    //    /* [in] */ const String& data);

    //CARAPI GetLength(
    //    /* [out] */ Int32 * pValue);

    //CARAPI SubstringData(
    //    /* [in] */ Int32 offset,
    //    /* [in] */ Int32 count,
    //    /* [out] */ String * pStr);

    //CARAPI AppendData(
    //    /* [in] */ const String& arg);

    //CARAPI InsertData(
    //    /* [in] */ Int32 offset,
    //    /* [in] */ const String& arg);

    //CARAPI DeleteData(
    //    /* [in] */ Int32 offset,
    //    /* [in] */ Int32 count);

    //CARAPI ReplaceData(
    //    /* [in] */ Int32 offset,
    //    /* [in] */ Int32 count,
    //    /* [in] */ const String& arg);

    CARAPI SplitText(
        /* [in] */ Int32 offset,
        /* [out] */ Org::W3c::Dom::IText ** ppText);

    CARAPI IsElementContentWhitespace(
        /* [out] */ Boolean * pValue);

    CARAPI GetWholeText(
        /* [out] */ String * pStr);

    CARAPI ReplaceWholeText(
        /* [in] */ const String& content,
        /* [out] */ Org::W3c::Dom::IText ** ppText);

    CARAPI Minimize(
        /* [out] */ TextImpl** result);
private:
    CARAPI_(AutoPtr<TextImpl>) FirstTextNodeInCurrentRun();

    CARAPI_(AutoPtr<TextImpl>) NextTextNode();
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_XML_DOM_TEXTIMPL_H__
