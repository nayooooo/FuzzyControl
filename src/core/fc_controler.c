#include "fc_controler.h"

bool fc_controler_register(struct fc_controler* fcc)
{
	if (fcc == nullptr) return false;

	fcc->in = list_create();
	if (fcc->in == nullptr) goto _error_out;
	fcc->out = list_create();
	if (fcc->out == nullptr) goto _error_out;
	fcc->rules = (struct fc_rules*)__FC_CONTROLER_MALLOC(sizeof(struct fc_rules));
	if (fcc->rules == nullptr) goto _error_out;
	if (!fc_rules_register(fcc->rules)) goto _error_out;;
	
	return true;

_error_out:

	if (fcc->in != nullptr)
	{
		list_delete(fcc->in, nullptr);
		fcc->in = nullptr;
	}
	if (fcc->out != nullptr)
	{
		list_delete(fcc->out, nullptr);
		fcc->out = nullptr;
	}
	if (fcc->rules != nullptr)
	{
		__FC_CONTROLER_FREE((void*)(fcc->rules));
		fcc->rules = nullptr;
	}

	return false;
}

bool fc_controler_unregister(struct fc_controler* fcc)
{
	if (fcc == nullptr) return false;

	if (fcc->in != nullptr)
	{
		list_delete(fcc->in, nullptr);
		fcc->in = nullptr;
	}
	if (fcc->out != nullptr)
	{
		list_delete(fcc->out, nullptr);
		fcc->out = nullptr;
	}
	if (fcc->rules != nullptr)
	{
		fc_rules_unregister(fcc->rules);
		__FC_CONTROLER_FREE((void*)(fcc->rules));
		fcc->rules = nullptr;
	}

	return true;
}
