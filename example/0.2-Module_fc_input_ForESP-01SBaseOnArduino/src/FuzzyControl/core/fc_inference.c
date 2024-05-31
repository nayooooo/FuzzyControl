#include "fc_inference.h"
#include "fc_def.h"

bool fc_inference_register(struct fc_inference* const infer, const char* name)
{
	if (infer == nullptr || name == nullptr) return false;

	if (!fc_interface_register(&(infer->interface), name, FC_OBJ_INFERENCE)) return false;

	infer->register_dev = fc_inference_register;
	infer->unregister_dev = fc_inference_unregister;
	infer->inference = nullptr;
	infer->print = nullptr;

	return false;
}

bool fc_inference_unregister(struct fc_inference* const infer)
{
	if (infer == nullptr) return false;
	
	if (!fc_interface_unregister(&(infer->interface))) return false;

	infer->register_dev = nullptr;
	infer->unregister_dev = nullptr;
	infer->inference = nullptr;
	infer->print = nullptr;

	return true;
}
