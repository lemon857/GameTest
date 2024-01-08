#include "EngineCore/Resources/RegistryManager.h"

#include "EngineCore/System/Log.h"

#include <Windows.h>

std::string RegistryManager::m_path;

void RegistryManager::set_reg_path(std::string path)
{
	m_path = "Software\\" + path;
}

int RegistryManager::set_value(char* name_value, char* value, int valueLen)
{
    HKEY hKey;

    // Создаем ключ в ветке HKEY_CURRENT_USER
    if (RegCreateKeyExA(HKEY_CURRENT_USER, m_path.c_str(), 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
        LOG_ERROR("[REG] Error create/get registry key");
        return 1;
    }

    // Пишем тестовую строку в созданный ключ
    if (RegSetKeyValueA(hKey, 0, name_value, REG_BINARY, (BYTE*)value, sizeof(value[0]) * valueLen) != ERROR_SUCCESS) {
        LOG_ERROR("[REG] Error write binary in registry");
        return 2;
    }

    // Закрываем описатель ключа
    if (RegCloseKey(hKey) != ERROR_SUCCESS) {
        LOG_ERROR("[REG] Error close key");
        return 3;
    };
	return 0;
}

bool RegistryManager::get_value(char* name_value, char* valueBuf, int bufLen)
{
    DWORD dwBufLen = bufLen;

    if (RegGetValueA(HKEY_CURRENT_USER, m_path.c_str(), name_value, RRF_RT_REG_BINARY, NULL, (BYTE*)valueBuf, &dwBufLen) != ERROR_SUCCESS) {
        LOG_ERROR("[REG] Error read binary from registry");
        return false;
    }    
    return true;
}

int RegistryManager::set_value(std::string name_value, std::string value)
{
    HKEY hKey;

    // Создаем ключ в ветке HKEY_CURRENT_USER
    if (RegCreateKeyExA(HKEY_CURRENT_USER, m_path.c_str(), 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
        LOG_ERROR("[REG] Error create/get registry key");
        return 1;
    }

    // Пишем тестовую строку в созданный ключ
    if (RegSetKeyValueA(hKey, 0, name_value.c_str(), REG_BINARY, (BYTE*)(value.c_str()), sizeof(value[0]) * value.size()) != ERROR_SUCCESS) {
        LOG_ERROR("[REG] Error write string in registry");
        return 2;
    }

    // Закрываем описатель ключа
    if (RegCloseKey(hKey) != ERROR_SUCCESS) {
        LOG_ERROR("[REG] Error close key");
        return 3;
    };
    return 0;
}

bool RegistryManager::get_value(std::string name_value, std::string& valueBuf)
{
    char szBuf[MAX_PATH];
    DWORD dwBufLen = MAX_PATH;

    if (RegGetValueA(HKEY_CURRENT_USER, m_path.c_str(), name_value.c_str(), RRF_RT_REG_BINARY, NULL, (BYTE*)szBuf, &dwBufLen) != ERROR_SUCCESS) {
        LOG_ERROR("[REG] Error read string from registry");
        return false;
    }
    valueBuf = std::string(szBuf);
    return true;
}
