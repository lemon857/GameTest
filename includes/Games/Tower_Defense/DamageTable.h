#pragma once

enum class TypeAttack
{
	Piercing = 0, // колющий
	Cutting, // режущий
	Shock, // шокирующий
	Sorcery, // колдовской
	Wither // иссушающий
};

enum class TypeArmor
{
	Null = 0, // нет
	Simple, // простая
	Heavy, // тяжёлая
	Magic, // магическая
	Chaotic // хаотическая
};

const double damageTable[5][5]
{// null simple heavy magic chaotic
	{1.1,	0.9, 0.7,	1.1,	0.8}, // Piercing
	{1, 	1.1, 0.8,	1,		0.9}, // Cutting
	{0.9, 	0.8, 1,		0.9,	1}, // Shock
	{0.8,	1,	 1,		0.9,	1.2}, // Sorcery
	{1,		0.9, 1.2,	0.8,	0.9} // Wither
};