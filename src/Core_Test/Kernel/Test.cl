__kernel void test(
	__constant int* in1,
	__constant int* in2,
	__global int* result)
{
	size_t work_id = get_global_id(0);
	result[work_id] = in1[work_id] + in2[work_id];
}