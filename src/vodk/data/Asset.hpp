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
    STRING_ASSET,
    SHADER_ASSET,
    SHADER_PROGRAM_ASSET,
    FILE_ASSET,
    TEXTURE_ASSET,
    IMAGE_ASSET,
    GEOMETRY_ASSET,
    SOUND_ASSET,
};

class AssetManager : public kiwi::Graph { };

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

    Asset(AssetManager* mgr)
    : kiwi::Node(mgr)
    , _on_loadCB(nullptr), _state(INVALID)
    {}

    virtual ~Asset()
    {
        on_load(nullptr);
    }

    State get_state() const {
        return _state;
    }

    AssetConnectionIterator dependencies() {
        return AssetConnectionIterator(inputConnections(), false);
    }
    AssetConnectionIterator dependent_assets() {
        return AssetConnectionIterator(outputConnections(), true);
    }

    bool add_dependency(Asset* dep, int port = 0) {
        assert(dep);
        return get_graph()->connect(*dep, 0, *this, port);
    }

    virtual AssetType get_type() = 0;

    virtual bool load() = 0;
    virtual void unload() = 0;

    void invalidate() {
        _state = DIRTY;
    }

    typedef core::Closure<Asset> LoadCallback;

    virtual void on_load(LoadCallback* callback) {
        if (_on_loadCB) {
            delete _on_loadCB;
        }
        _on_loadCB = callback;
    }
protected:
    void notify_loaded() {
        if (_on_loadCB) {
            _on_loadCB->call(*this);
        }
        _state = LOADED;
    }
    void set_state(State s) {
        _state = s;
    }
private:
    LoadCallback* _on_loadCB;
    State _state;
};

template<typename T> 
T* cast_asset(Asset* a) {
    if (a->get_type() == T::Type) return static_cast<T*>(a);
    return nullptr;
}


} // namespace data
} // namespace vodk

#endif
