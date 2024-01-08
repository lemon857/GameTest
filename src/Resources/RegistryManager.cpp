#include "EngineCore/Resources/RegistryManager.h"

#include "EngineCore/System/Log.h"

#include <Windows.h>

std::string RegistryManager::m_path;

void RegistryManager::set_reg_path(std::string path)
{
	m_path = path;
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
    if (RegSetKeyValueA(hKey, 0, name_value.c_str(), REG_SZ, (BYTE*)(value.c_str()), sizeof(value.c_str())) != ERROR_SUCCESS) {
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

std::string RegistryManager::get_value(std::string name_value)
{
    char szBuf[MAX_PATH];
    DWORD dwBufLen = MAX_PATH;

    if (RegGetValueA(HKEY_CURRENT_USER, m_path.c_str(), name_value.c_str(), RRF_RT_REG_SZ, NULL, (BYTE*)szBuf, &dwBufLen) != ERROR_SUCCESS) {
        LOG_ERROR("[REG] Error read string from registry");
        return "";
    }
    return std::string(szBuf);
}
