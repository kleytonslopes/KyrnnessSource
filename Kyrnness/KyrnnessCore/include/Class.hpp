#pragma once

#ifndef KYRNNESS_CLASS_HPP
#define KYRNNESS_CLASS_HPP

class UClass
{
public:
	virtual void Initialize() = 0;
	virtual void PostInitialize() = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void Destroy() = 0;
};

#endif // KYRNNESS_CLASS_HPP;
