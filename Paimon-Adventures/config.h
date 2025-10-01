#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <Windows.h>

class ConfigItem{
    std::string value="0";
public:
    ConfigItem() = default;
    ConfigItem(const std::string& val):value(val){}
    ConfigItem(const char* val):value(val){}
    ConfigItem(int val):value(std::to_string(val)){}
    ConfigItem(float val):value(std::to_string(val)){}
    ConfigItem(bool val):value(val?"true":"false"){}

    std::string asString() const { return value; }
    int asInt() const {
        try {
            return std::stoi(value);
        } catch (...) {
            std::cerr << "Invalid int value: " << value << std::endl;
            return 0;
        }
    }
    float asFloat() const {
        try {
            return std::stof(value);
        } catch (...) {
            std::cerr << "Invalid float value: " << value << std::endl;
            return 0.0f;
        }
    }
    bool asBool() const {
        if (value == "true" || value == "1") return true;
        if (value == "false" || value == "0") return false;
        std::cerr << "Invalid bool value: " << value << std::endl;
        return false;
    }
    ConfigItem& operator=(const std::string& val) { value = val; return *this; }
    ConfigItem& operator=(const char* val) { value = val; return *this; }
    ConfigItem& operator=(int val) { value = std::to_string(val); return *this; }
    ConfigItem& operator=(float val) { value = std::to_string(val); return *this; }
    ConfigItem& operator=(bool val) { value = val ? "true" : "false"; return *this; }

    operator std::string() const { return value; }
    operator int() const { return asInt(); }
    operator float() const { return asFloat(); }
    operator bool() const { return asBool(); }
};

class Config {
    std::unordered_map<std::string, ConfigItem> data;
    const std::string filename = "config.txt";
    Config* default_instance = nullptr;
    bool is_default = false;
public:
    Config(bool is_default = false) : is_default(is_default) {
        if(is_default)return;
        default_instance = new Config(true);
        load();
        setDefaults();
        printAll();
    }
    ~Config() {
        save();
    }
    const ConfigItem& operator[](const std::string& key) const {
        auto it = data.find(key);
        if (it != data.end()) {
            return it->second;
        }else{
            if(is_default){
                static ConfigItem empty;
                return empty;
            }
            return (*default_instance)[key];
        }
    }

    const ConfigItem& get(const std::string& key) const {return (*this)[key];}
    void set(const std::string& key, const ConfigItem& value) {data[key] = value;}
    void setifno(const std::string& key, const ConfigItem& value) {
        if(data.find(key) == data.end())
            data[key] = value;
        default_instance->set(key, value);
    }
    void save(){
        if(is_default)return;
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Could not open config file for writing: " << filename << std::endl;
            return;
        }

        for (const auto& pair : data) {
            file << pair.first << "=" << pair.second.asString() << std::endl;
        }
        file.close();
    }
    void load(){
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Could not open config file: " << filename << std::endl;
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                data[key] = value;
            }
        }
        file.close();
    }
    void setDefaults(){
        setifno("player.warrior.speed", 0.4f);
        setifno("player.warrior.hp", 100);
        setifno("player.warrior.around_bullets", 4);
        setifno("player.warrior.around_bullets_addMp", 10);
        setifno("player.warrior.skill_addMp", 5);
        setifno("player.custom_bullets.speed", 0.7f);

        setifno("player.paimon.speed", 0.45f);
        setifno("player.paimon.hp", 80);
        setifno("player.paimon.around_bullets", 4);
        setifno("player.paimon.around_bullets_addMp", 10);
        setifno("player.paimon.skill_addMp", 5);

        setifno("player.init.position.x", 640);
        setifno("player.init.position.y", 360);
        setifno("player.maxMp", 100);
        setifno("player.invincibleTime", 1000); // milliseconds
        setifno("player.shadow_offset_y", 8);
        setifno("player.key.up1", 'W');
        setifno("player.key.up2", VK_UP);
        setifno("player.key.down1", 'S');
        setifno("player.key.down2", VK_DOWN);
        setifno("player.key.left1", 'A');
        setifno("player.key.left2", VK_LEFT);
        setifno("player.key.right1", 'D');
        setifno("player.key.right2", VK_RIGHT);
        setifno("player.key.skill1", 'F');
        setifno("player.key.skill2", VK_SPACE);

        setifno("bullet.radial_speed", 0.0045f);
        setifno("bullet.tangent_speed", 0.0015f);

        setifno("enemy.bee.speed", 0.5f);
        setifno("enemy.bee.hp", 1);
        setifno("enemy.boar.speed", 0.25f);
        setifno("enemy.boar.hp", 3);
        setifno("enemy.snail.speed", 0.1f);
        setifno("enemy.snail.hp", 5);
        setifno("enemy.bee.invincibleTime", 100);
        setifno("enemy.boar.invincibleTime", 300);
        setifno("enemy.snail.invincibleTime", 600);
        setifno("enemy.bee.damage", 5);
        setifno("enemy.boar.damage", 10);
        setifno("enemy.snail.damage", 5);
        setifno("enemy.shadow_offset_y", 35);
        setifno("enemy.initial_summon_interval", 1000);
        setifno("enemy.summon_interval_decrease", 10); // decrease 1ms per score
        setifno("enemy.least_summon_interval", 100); // milliseconds

        setifno("debug", false);
        setifno("pos.status_bar.x", 20);
        setifno("pos.status_bar.y", 10);
    }
    void printAll(){
        for (const auto& pair : data) {
            std::cout << pair.first << "=" << pair.second.asString() << std::endl;
        }
    }
};
extern Config config;