#pragma once
#include "../Components/PublicVariable.h"
#include <string>
#include <vector>

namespace glGame {

    enum class AssetType {
        Model, Texture, Script, Shader,
        None
    };

    namespace assetInternal {
        class AssetT {
        public:
            AssetT(const char* filepath) : filepath(filepath) {}

            virtual const char* getAssetTypeString() = 0;

        protected:
            void addPublicVariable(glGame::PublicVarVariant data, std::string name) { m_publicVariables.push_back(glGame::PublicVariable(data, name)); }
		    void addPublicVariable(glGame::PublicVarVariant data, std::string name, const float& sliderSpeed) { m_publicVariables.push_back(glGame::PublicVariable(data, name, sliderSpeed)); }

        public:
            std::string filepath;
            std::vector<glGame::PublicVariable> m_publicVariables;
        };

        template<class T>
        class Asset : public AssetT {
        public:
            Asset(const char* filepath) : AssetT(filepath), asset(filepath) { }

            virtual const char* getAssetTypeString() override { return ""; }
            static AssetType GetAssetType() { return AssetType::None; }

            T* get() { return &asset; }
            T* operator->() { return &asset; }
        private:
            T asset;
        };
    }
}