#include "ConfigManager.hpp"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <sstream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace boost;

void ConfigManager::set_path(const std::string & path)
{
  try
  {
    path_ = filesystem::path(path);
    if (filesystem::exists(path_))
    {
      if (!filesystem::is_directory(path_))
        throw SystemException("Configuration Manager: Path is not a directory.");
    }
    else
      throw SystemException("Configuration Manager: Directory don't exist.");
  }
  catch (filesystem::filesystem_error& ex)
  {
    throw SystemException("Configuration Manager: " + std::string(ex.what()));
  }
}

void ConfigManager::load_config(const std::string & config_name)
{
  try
  {
    filesystem::path filepath_ = path_ / filesystem::path(config_name + ".cfg");

    if (!filesystem::exists(filepath_))
      throw SystemException("Configuration Manager: Configuration file don't exist.");
 
    std::ifstream config_file(filepath_.generic_string());

    if (config_file.good() == false)
      throw SystemException("Configuration Manager: Con't open configuration file.");

    std::stringstream config_stream;
    config_stream << config_file.rdbuf();

    config_file.close();

    rapidjson::Document temp;
    temp.Parse(config_stream.str().c_str());

    if (temp.HasParseError() || !temp.IsObject())
      throw ConfigException("Configuration Manager: Configuration file parse error.");

    json_config_ = std::move(temp);
    config_name_ = config_name;

    config_loaded_ = true;
  }
  catch (filesystem::filesystem_error& ex)
  {
    throw SystemException("Configuration Manager: " + std::string(ex.what()));
  }
  catch (...)
  {
    throw;
  }
}

void ConfigManager::save_config(const std::string & config_name) const
{
  if (config_loaded_ == false)
    throw ConfigException("Configuration Manager: Config not loaded.");

  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  json_config_.Accept(writer);
  const std::string save_str = buffer.GetString();

  try
  {
    std::ofstream new_config(path_.generic_string() + std::string(config_name + ".cfg"));
    new_config << save_str;
    new_config.close();
  }
  catch (const std::exception& ex)
  {
    throw SystemException("Configuration Manager: " + std::string(ex.what()));
  }
}

void ConfigManager::update_config(const std::string & config_str)
{
  if (config_loaded_ == false)
    throw ConfigException("Configuration Manager: Config not loaded.");

  rapidjson::Document temp;
  temp.Parse(config_str.c_str());

  if (temp.HasParseError() || !temp.IsObject())
    throw ConfigException("Configuration Manager: Configuration update error.");

  json_config_ = std::move(temp);
}

std::string ConfigManager::config_name() const noexcept
{
  if (config_loaded_ == false)
    return std::string();

  return config_name_;
}

std::string ConfigManager::config_path() const noexcept
{
  if (config_loaded_ == false)
    return std::string();

  return path_.generic_string();
}

std::string ConfigManager::get_config_str() const noexcept
{
  if (config_loaded_ == false)
    return std::string();

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  json_config_.Accept(writer);
  
  return buffer.GetString();
}

ConfigManager::Settings ConfigManager::get_section(const std::string & section_name) const
{
  if (config_loaded_ == false)
    throw ConfigException("Configuration Manager: Config not loaded.");

  if (json_config_.HasMember(section_name.c_str()))
  {
    if (json_config_[section_name.c_str()].IsObject())
    {
      return json_config_[section_name.c_str()];
    }
    else
      throw ConfigException("Configuration Manager: Unknown config section name.");
  }
  else
    throw ConfigException("Configuration Manager: Unknown config section name.");
}

ConfigManager::Object ConfigManager::get_object(const Settings settings, const char * key) const
{
  if (config_loaded_ == false)
    throw ConfigException("Configuration Manager: Config not loaded.");

  if (settings.HasMember(key))
  {
    if (settings[key].IsObject() || settings[key].IsArray())
    {
      return settings[key];
    }
    else
      throw ConfigException("Configuration Manager: Not an object.");
  }
  else
    throw ConfigException("Configuration Manager: Unknown object name.");
}

std::vector<ConfigManager::ObjectPtr> ConfigManager::get_object_array(const Settings settings, const char* key) const
{
    if (config_loaded_ == false)
      throw ConfigException("Configuration Manager: Config not loaded.");

    if (key != nullptr)
    {
      if (settings.HasMember(key))
      {
        std::vector<ConfigManager::ObjectPtr> ret;
        auto& obj = settings[key];

        if (!obj.IsArray())
            throw ConfigException("Configuration Manager: Parse error.");

        for (rapidjson::Value::ConstValueIterator itr = obj.Begin(); itr != obj.End(); ++itr)
        {
            if (!itr->IsObject())
                throw ConfigException("Configuration Manager: Parse error.");
            ret.push_back(itr);
        }

        return ret;
      }
      else
        throw ConfigException("Configuration Manager: Parse error.");
    }
    else
    {
      throw ConfigException("Configuration Manager: Parse error.");
    }
}

void ConfigManager::set_value(Settings settings, const char * key, const std::string & value)
{
  if (config_loaded_ == false)
    throw ConfigException("Configuration Manager: Config not loaded.");

  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsString())
    {
      auto& obj = const_cast<rapidjson::Value&>(settings[key]);
      obj.SetString(value.c_str(), value.size(), json_config_.GetAllocator());
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    throw ConfigException("Configuration Manager: Parse error.");
  }
}

void ConfigManager::set_value(Settings settings, const char * key, const double value)
{
  if (config_loaded_ == false)
    throw ConfigException("Configuration Manager: Config not loaded.");

  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsDouble())
    {
      auto& obj = const_cast<rapidjson::Value&>(settings[key]);
      obj.SetDouble(value);
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    throw ConfigException("Configuration Manager: Parse error.");
  }
}

void ConfigManager::set_value(Settings settings, const char * key, const std::uint32_t value)
{
  if (config_loaded_ == false)
    throw ConfigException("Configuration Manager: Config not loaded.");

  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsUint())
    {
      auto& obj = const_cast<rapidjson::Value&>(settings[key]);
      obj.SetUint(value);
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    throw ConfigException("Configuration Manager: Parse error.");
  }
}

void ConfigManager::set_value(Settings settings, const char * key, const std::uint64_t value)
{
  if (config_loaded_ == false)
    throw ConfigException("Configuration Manager: Config not loaded.");

  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsUint64())
    {
      auto& obj = const_cast<rapidjson::Value&>(settings[key]);
      obj.SetUint64(value);
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    throw ConfigException("Configuration Manager: Parse error.");
  }
}

void ConfigManager::set_value(Settings settings, const char * key, const std::int32_t value)
{
  if (config_loaded_ == false)
    throw ConfigException("Configuration Manager: Config not loaded.");

  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsInt())
    {
      auto& obj = const_cast<rapidjson::Value&>(settings[key]);
      obj.SetInt(value);
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    throw ConfigException("Configuration Manager: Parse error.");
  }
}

void ConfigManager::set_value(Settings settings, const char * key, const std::int64_t value)
{
  if (config_loaded_ == false)
    throw ConfigException("Configuration Manager: Config not loaded.");

  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsInt64())
    {
      auto& obj = const_cast<rapidjson::Value&>(settings[key]);
      obj.SetInt64(value);
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    throw ConfigException("Configuration Manager: Parse error.");
  }
}

template<typename RetType>
RetType ConfigManager::get_value(const Settings settings, const char * key) const
{
  // Not implemented for generic type
  assert(false);
  return RetType();
}

template<>
std::string ConfigManager::get_value<std::string>(const Settings settings, const char* key) const
{
  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsString())
    {
      return settings[key].GetString();
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    if (settings.IsString())
      return settings.GetString();
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
}

template<>
double ConfigManager::get_value<double>(const Settings settings, const char* key) const
{
  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsNumber())
      return settings[key].GetDouble();
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    if (settings.IsNumber())
      return settings.GetDouble();
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
}

template<>
std::uint32_t ConfigManager::get_value<std::uint32_t>(const Settings settings, const char* key) const
{
  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsUint())
    {
      return settings[key].GetUint();
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    if (settings.IsUint())
    {
      return settings.GetUint();
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
}

template<>
std::int32_t ConfigManager::get_value<std::int32_t>(const Settings settings, const char* key) const
{
  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsInt())
      return settings[key].GetInt();
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    if (settings.IsInt())
      return settings.GetInt();
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
}

template<>
std::uint64_t ConfigManager::get_value<std::uint64_t>(const Settings settings, const char* key) const
{
  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsUint64())
    {
      return settings[key].GetUint64();
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    if (settings.IsUint64())
      return settings.GetUint64();
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
}

template<>
std::int64_t ConfigManager::get_value<std::int64_t>(const Settings settings, const char* key) const
{
  if (key != nullptr)
  {
    if (settings.HasMember(key) && settings[key].IsInt64())
    {
      return settings[key].GetInt64();
    }
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
  else
  {
    if (settings.IsInt64())
      return settings.GetInt64();
    else
      throw ConfigException("Configuration Manager: Parse error.");
  }
}

template<typename RetType>
std::vector<RetType> ConfigManager::get_array(const Object object) const
{
  if (object.IsArray())
  {
    std::vector<RetType> ret;
    for (rapidjson::SizeType i = 0; i < object.Size(); i++)
    {
      ret.push_back(get_value<RetType>(object[i]));
    }
    return ret;
  }
  else
  {
    throw ConfigException("Configuration Manager: Parse error.");;
  }
}
