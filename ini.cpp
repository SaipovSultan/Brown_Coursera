#include "ini.h"

namespace Ini{
    Section& Document::AddSection(std::string name) {
        return sections[std::move(name)];
    }
    const Section& Document::GetSection(const std::string &name) const {
        return sections.at(name);
    }
    size_t Document::SectionCount() const {
        return sections.size();
    }
    Document Load(std::istream& input){
        Document document;
        Section* section = nullptr;
        for(std::string line;std::getline(input, line);){
            if(!line.empty()){
                if(line[0] == '['){
                    section = &document.AddSection(line.substr(1, line.size() - 2));
                }else{
                    size_t pos = line.find('=');
                    section->insert({line.substr(0, pos), line.substr(pos + 1)});
                }
            }
        }
        return document;
    }
}