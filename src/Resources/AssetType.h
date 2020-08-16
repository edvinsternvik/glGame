#pragma once
#include "../Components/PublicVariable.h"
#include <string>
#include <vector>

namespace glGame {

    enum class AssetType {
        Model, Texture, Script, Shader, Material,
        None
    };

    namespace assetInternal {
        class AssetT {
        public:
            AssetT(const char* filepath) : filepath(filepath) {}

            virtual const char* getAssetTypeString() = 0;
            virtual bool hasData() = 0;
            virtual void reload() {}

            const int getPublicVariableCount() const {
			    return m_publicVariables.size();
            }

            const PublicVariable* getPublicVariable(int index) const {
                if(index < 0 || index >= m_publicVariables.size()) return nullptr;
                return &(m_publicVariables[index]);
            }

            PublicVariable* const getPublicVariable(const std::string& name) {
                for(PublicVariable& pVar : m_publicVariables) {
                    if(name == pVar.name) return &pVar;
                }

                return nullptr;
            }

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
            virtual void reload() override {}
            static AssetType GetAssetType() { return AssetType::None; }

            T* get() { return &asset; }
            T* operator->() { return &asset; }
        private:
            T asset;
        };
    }
}