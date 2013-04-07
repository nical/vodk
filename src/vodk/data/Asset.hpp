#ifndef VODK_DATA_ASSET_HPP
#define VODK_DATA_ASSET_HPP

#include "vodk/core/Closure.hpp"
#include <stdint.h>
#include <assert.h>

#include "kiwi/Node.hpp"

namespace vodk {

namespace gfx {
class Image;
} // namespace

namespace data {

class Asset;

enum AssetType {
    SHADER_ASSET,
    SHADER_PROGRAM_ASSET,
    FILE_ASSET,
    TEXTURE_ASSET,
    IMAGE_ASSET,
    GEOMETRY_ASSET,
    SOUND_ASSET,
};

struct AssetID {
    const char* _name;
};

class AssetConnectionIterator
{
public:
    bool empty() {
        return _it.empty();
    }
    AssetConnectionIterator next() {
        return AssetConnectionIterator(_it.next(), _isInput);
    }
    Asset& operator*();
    AssetConnectionIterator(const kiwi::LinkIterator& it, bool isInput)
    : _it(it), _isInput(isInput) {}
private:
    kiwi::LinkIterator _it;
    bool _isInput;
};

class Asset : public kiwi::Node
{
public:
    enum State {
        LOADED,
        DIRTY,
        DEFAULT,
        INVALID,
    };

    Asset()
    : _onLoadCB(nullptr), _state(INVALID)
    {}

    virtual ~Asset()
    {
        onLoad(nullptr);
    }

    State getState() const {
        return _state;
    }

    AssetConnectionIterator dependencies() {
        return AssetConnectionIterator(inputConnections(), false);
    }
    AssetConnectionIterator dependentAssets() {
        return AssetConnectionIterator(outputConnections(), true);
    }

    bool addDependency(Asset* dep) {
        assert(dep);
        return kiwi::Node::connect(*dep, 0, *this, 0);
    }

    virtual AssetType getType() = 0;

    virtual bool load() = 0;
    virtual void unload() = 0;

    void invalidate() {
        _state = DIRTY;
    }

    typedef core::Closure<Asset> LoadCallback;

    virtual void onLoad(LoadCallback* callback) {
        if (_onLoadCB) {
            delete _onLoadCB;
        }
        _onLoadCB = callback;
    }
protected:
    void notifyLoaded() {
        if (_onLoadCB) {
            _onLoadCB->call(*this);
        }
        _state = LOADED;
    }
    void setState(State s) {
        _state = s;
    }
private:
    LoadCallback* _onLoadCB;
    State _state;
};

} // namespace data
} // namespace vodk

#endif
