#include "Liner.hlsli"

VSOutput main(VSOutput vin) : SV_POSITION
{
	VSOutput vout;
	vout.position = mul(vin.position, world_);
	return vout;
}