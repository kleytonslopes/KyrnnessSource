#include "pch.hpp"
#include "Components/IdentityComponent.hpp"
#include <random>
#include <sstream>
#include <iomanip>
#include "Core/Guid.hpp"

FIdentityComponent::FIdentityComponent(const std::string& objectName) : Name(objectName)
{
	GenerateId();
}

void FIdentityComponent::GenerateId()
{
	Id = FGuid::NewGuid().ToString();
}
