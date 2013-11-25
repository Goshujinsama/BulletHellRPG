#ifndef COMMON_H
#define COMMON_H

#define SAFE_DELETE(x) { if(x != NULL) { delete(x); x = NULL; } }
#define SAFE_DELETE_ARRAY(x) { if(x != NULL) { delete [] (x); x = NULL; } }
#define SAFE_RELEASE(x)  { if(x != NULL) { x->Release(); x = NULL; } }
#define RETURN_FAIL(x) { if(FAILED(x)) { return false; } }
#define RETURN_ERROR(x) { if((x) != 0) { return false; } }

#endif