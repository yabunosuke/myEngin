#include "Liner.hlsli"

VSOutput main(VSInput vin)
{
	VSOutput vout;
	vout.position = mul(vin.position, world_);
	return vout;
}