/*	Public domain	*/

#ifndef	_AGAR_CORE_ERROR_H_
#define	_AGAR_CORE_ERROR_H_

#include <agar/config/_mk_have_stdlib_h.h>
#ifdef _MK_HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <agar/core/begin.h>

/* Standard error code */
typedef enum ag_error_code {
	AG_EUNDEFINED,		/* Undefined error */
	AG_EPERM,		/* Operation not permitted */
	AG_ENOENT,		/* No such file or directory */
	AG_EINTR,		/* Interrupted system call */
	AG_EIO,			/* Input/output error */
	AG_E2BIG,		/* Argument list too long */
	AG_EACCESS,		/* Permission denied */
	AG_EBUSY,		/* Device or resource busy */
	AG_EEXIST,		/* File exists */
	AG_ENOTDIR,		/* Not a directory */
	AG_EISDIR,		/* Is a directory */
	AG_EMFILE,		/* Too many open files */
	AG_EFBIG,		/* File too large */
	AG_ENOSPC,		/* No space left on device */
	AG_EROFS,		/* Read-only file system */
	AG_EAGAIN		/* Resource temporarily unavailable */
} AG_ErrorCode;

#if defined(_AGAR_INTERNAL) || defined(_USE_AGAR_STD)
# define Malloc(len) AG_Malloc(len)
# define TryMalloc(len) AG_TryMalloc(len)
# define Free(p) AG_Free(p)
# define Realloc(p,len) AG_Realloc((p),(len))
# define TryRealloc(p,len) AG_TryRealloc((p),(len))
# define Snprintf AG_Snprintf
# define Vsnprintf AG_Vsnprintf
# define Vasprintf(msg, fmt, args) do { if (AG_Vasprintf((msg),(fmt),(args)) == -1) AG_FatalError("Out of memory (vasprintf)"); } while (0)
# define TryVasprintf(msg, fmt, args) AG_Vasprintf((msg),(fmt),(args))
# define Verbose AG_Verbose
# ifdef AG_DEBUG
#  define Debug AG_Debug
# else
#  ifdef __GNUC__
#   define Debug(obj, arg...) ((void)0)
#  else
#   define Debug AG_Debug
#  endif
# endif /* AG_DEBUG */
#endif /* _AGAR_INTERNAL or _USE_AGAR_STD */

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC int agDebugLvl;
extern DECLSPEC void AG_InitError(void);
extern DECLSPEC void AG_DestroyError(void);
extern DECLSPEC AG_ErrorCode AG_GetErrorCode(void);
extern DECLSPEC const char *AG_GetError(void);
extern DECLSPEC void AG_SetErrorCode(AG_ErrorCode);
extern DECLSPEC void AG_SetError(const char *, ...) FORMAT_ATTRIBUTE(printf, 1, 2) NONNULL_ATTRIBUTE(1);
extern DECLSPEC void AG_FatalError(const char *, ...) NORETURN_ATTRIBUTE;
extern DECLSPEC void AG_SetFatalCallback(void (*callback)(const char *));
extern DECLSPEC void AG_Debug(void *, const char *, ...) FORMAT_ATTRIBUTE(printf, 2, 3) NONNULL_ATTRIBUTE(2);
extern DECLSPEC void AG_Verbose(const char *, ...) FORMAT_ATTRIBUTE(printf, 1, 2) NONNULL_ATTRIBUTE(1);
extern DECLSPEC void *AG_PtrMismatch(void);
extern DECLSPEC void *AG_ObjectMismatch(const char *, const char *);
extern DECLSPEC int AG_IntMismatch(void);
extern DECLSPEC float AG_FloatMismatch(void);
extern DECLSPEC const char *AG_Strerror(int);

static __inline__ void *
AG_Malloc(size_t len)
{
	void *p;
	if ((p = malloc(len)) == NULL) { AG_FatalError("malloc"); }
	return (p);
}

static __inline__ void *
AG_TryMalloc(size_t len)
{
	void *p;
	if ((p = malloc(len)) == NULL) {
		AG_SetError("Out of memory");
		return (NULL);
	}
	return (p);
}

static __inline__ void *
AG_Realloc(void *pOld, size_t len)
{
	void *pNew;
	
	if (pOld == NULL) {
		if ((pNew = malloc(len)) == NULL)
			AG_FatalError("malloc");
	} else {
		if ((pNew = realloc(pOld, len)) == NULL)
			AG_FatalError("realloc");
	}
	return (pNew);
}

static __inline__ void *
AG_TryRealloc(void *pOld, size_t len)
{
	void *pNew;
	
	if (pOld == NULL) {
		if ((pNew = malloc(len)) == NULL)
			goto outofmem;
	} else {
		if ((pNew = realloc(pOld, len)) == NULL)
			goto outofmem;
	}
	return (pNew);
outofmem:
	AG_SetError("Out of memory");
	return (NULL);
}
#define AG_Free(p) free(p)
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
#endif /* _AGAR_CORE_ERROR_H_ */
