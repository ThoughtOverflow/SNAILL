// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageHandler.h"


// Add default functionality here for any IDamageHandler functions that are not pure virtual.

void IDamageHandler::SetObjectHealth(float newHealth)
{
}

void IDamageHandler::ChangeObjectHealth(float deltaHealth)
{
	SetObjectHealth(deltaHealth);
}
