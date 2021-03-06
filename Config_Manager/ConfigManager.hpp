#pragma once

#include <string>
#include <rapidjson/document.h>
#include <boost/filesystem.hpp>
#include "ErrorExceptions.h"

/**
 * @class ConfigManager
 *
 * @brief Exceptions caused by errors in the operating system functionality or 3rd party libraries.
 *
 */

class ConfigManager
{

  /** @brief Loaded JSON configuration. */
  rapidjson::Document json_config_;

  /** @brief Name of the configuration. */
  std::string config_name_;

  /** @brief Pathname of the directory with configuration files. */
  boost::filesystem::path path_;

  /** @brief true if configuration loaded. */
  bool config_loaded_ = false;


public:

  /** @brief Section's settings type. */
  using Settings = const rapidjson::Value&;

  /** @brief JSON object type. */
  using Object = const rapidjson::Value&;

  /** @brief JSON pointer to object type. */
  using ObjectPtr = const rapidjson::Value*;

  /**
   * @fn  void ConfigManager::set_path(const std::string& path);
   *
   * @brief Sets a path to directory with configuration files.
   *
   * @exception SystemException Thrown if the path don't exists.
   *
   * @param path  Full pathname of the file.
   */

  void set_path(const std::string& path);

  /**
   * @fn  void ConfigManager::load_config(const std::string& config_name);
   *
   * @brief Loads a configuration by name.
   *
   * @exception SystemException Thrown if the config file don't exists.
   * @exception ConfigException Thrown if a parse error occurs.
   *
   * @param config_name Name of the configuration.
   */

  void load_config(const std::string& config_name);

  /**
   * @fn  void ConfigManager::save_config(const std::string& config_name) const;
   *
   * @brief Saves configuration in a file.
   *
   * @exception SystemException Thrown if can't create file or write in a file.
   *
   * @param config_name Name of the configuration.
   */

  void save_config(const std::string& config_name) const;

  /**
   * @fn  void ConfigManager::update_config(const std::string& config_str);
   *
   * @brief Updates the configuration described by JSON config_str. All unsaved changes will be lost.
   *
   * @exception ConfigException Thrown if a parse error occurs.
   *
   * @param config_str  The configuration JSON string.
   */

  void update_config(const std::string& config_str);

  /**
   * @fn  std::string ConfigManager::config_name() const noexcept;
   *
   * @brief Current configuration name.
   *
   * @return  configuration name.
   */

  std::string config_name() const noexcept;

  /**
   * @fn  std::string ConfigManager::config_path() const noexcept;
   *
   * @brief Current configuration path.
   *
   * @return  configuration path.
   */

  std::string config_path() const noexcept;

  /**
   * @fn  std::string ConfigManager::get_config_str() const noexcept;
   *
   * @brief Gets configuration JOSN string.
   *
   * @return  The configuration JSON string.
   */

  std::string get_config_str() const noexcept;

  /**
   * @fn  Settings ConfigManager::get_section(const std::string& section_name) const;
   *
   * @brief Gets the settings for specific section.
   *
   * @exception ConfigException Thrown if a section don't exists.
   *
   * @param section_name  Name of the section.
   *
   * @return  The section settings.
   */

  const Settings get_section(const std::string& section_name) const;

  /**
   * @fn  Object ConfigManager::get_object(const Settings& settings, const char* key) const;
   *
   * @brief Gets an JSON object by key.
   *
   * @exception ConfigException Thrown if the key don't exists or value by key not an array or an object.
   *
   * @param settings  Section settings.
   * @param key       The key.
   *
   * @return  The JSON object (array or object).
   */

  Object get_object(const Settings settings, const char* key) const;

  /**
   * @fn  template<typename RetType> RetType ConfigManager::get_value(const Settings& settings, const char* key) const;
   *
   * @brief Gets a value of given type. Specialized for particular types.
   *
   * @exception ConfigException Thrown if the key don't exists or can't cast to RetType.
   *
   * @tparam  RetType Type of the return value.
   * @param settings  Section settings or Object.
   * @param key       The key.
   *
   * @return  The value of type RetType.
   */

  template<typename RetType> 
  RetType get_value(const Settings settings, const char* key = nullptr) const;

  /**
   * @fn  std::vector<Object> get_object_array(const Settings settings, const char* key = nullptr) const;
   *
   * @brief Gets an array of JSON objects by name for specific section.
   *
   * @exception ConfigException Thrown if the key don't exists or values in the array is not JSON objects.
   *                            
   * @param settings  Section settings or Object.
   * @param key       The key.
   *
   * @return  A std::vector&ltObject&gt;
   */
  std::vector<ObjectPtr> get_object_array(const Settings settings, const char* key = nullptr) const;

  /**
   * @fn  template<typename RetType, std::size_t Size> std::array<RetType, Size> ConfigManager::get_array(const Object& settings) const;
   *
   * @brief Converts the object to an array.
   *
   * @exception ConfigException Thrown if the object can't be cast to an array.
   *
   * @tparam  RetType  Type of the array.
   * @tparam   Size    Size of the array.
   * @param   object   JSON Object.
   *
   * @return  A std::array&lt;RetType,Size&gt;
   */

  template<typename RetType>
  std::vector<RetType> get_array(const Object object) const;

  /**
   * @fn  void ConfigManager::set_value(const Settings& settings, const char* key, const ValueType& value);
   *
   * @brief Sets a value. Overloaded for different values. Not a template.
   *
   * @exception ConfigException Thrown if value by key don't exists.
   *
   * @tparam  ValueType Value type parameter.
   * @param settings  Section settings or object.
   * @param key       The key.
   * @param value     The value.
   */

  void set_value(Settings settings, const char* key, const std::string& value);

  void set_value(Settings settings, const char* key, const double value);

  void set_value(Settings settings, const char* key, const std::uint32_t value);

  void set_value(Settings settings, const char* key, const std::int32_t value);

  void set_value(Settings settings, const char* key, const std::uint64_t value);

  void set_value(Settings settings, const char* key, const std::int64_t value);


  /**
   * @fn  template<typename ArrayType> void ConfigManager::set_array(Object& object, ArrayType arr);
   *
   * @brief Sets an array.
   *
   * @exception ConfigException Thrown if object is not an JSON array.
   *
   * @tparam  ArrayType Type of the array.
   * @param [in,out]  object  The JSON object.
   * @param arr               The array.
   */

  template<typename ArrayType> 
  void set_array(Object object, const std::vector<ArrayType>& arr);
};

/* Specializations */
  template<>
  std::string ConfigManager::get_value<std::string>(const Settings settings, const char* key) const;

  template<>
  double ConfigManager::get_value<double>(const Settings settings, const char* key) const;

  template<>
  std::uint32_t ConfigManager::get_value<std::uint32_t>(const Settings settings, const char* key) const;

  template<>
  std::int32_t ConfigManager::get_value<std::int32_t>(const Settings settings, const char* key) const;

  template<>
  std::uint64_t ConfigManager::get_value<std::uint64_t>(const Settings settings, const char* key) const;

  template<>
  std::int64_t ConfigManager::get_value<std::int64_t>(const Settings settings, const char* key) const;


template<typename ArrayType>
void ConfigManager::set_array(Object object, const std::vector<ArrayType>& arr)
{
  if (object.IsArray())
  {
    auto& obj = const_cast<rapidjson::Value&>(object);
    obj.Clear();
    for (auto& item : arr)
    {
      obj.PushBack(item, json_config_.GetAllocator());
    }
  }
  else
  {
    throw ConfigException("Configuration Manager: Parse error.");;
  }
}
