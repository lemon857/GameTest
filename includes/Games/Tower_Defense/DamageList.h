#pragma once

enum class TypeAttack
{
	Piercing = 0,
	Cutting,
	Heavy,
	Magic,
	Chaotic
};

enum class TypeArmor
{
	Null = 0,
	Simple,
	Heavy,
	Magic,
	Chaotic
};

const double damageList[5][5]
{// null simple heavy magic chaotic
	{1.1,	0.9, 0.7,	1.1,	0.8}, // Piercing
	{1, 	1.1, 0.8,	1,		0.9}, // Cutting
	{0.9, 	0.8, 1,		0.9,	1}, // Heavy
	{0.8,	1,	 1,		1,		1.2}, // Magic
	{1,		0.9, 1.2,	0.8,	0.9} // Chaotic
};