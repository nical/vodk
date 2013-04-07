
#include "vodk/data/Asset.hpp"

namespace vodk {
namespace data {

Asset& AssetConnectionIterator::operator*() {
    return *static_cast<Asset* const>( _isInput ? _it->inputNode
                                                : _it->outputNode);
}


} // namespace
} // namespace
