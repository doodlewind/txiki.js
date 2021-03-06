/*
 * QuickJS libuv bindings
 *
 * Copyright (c) 2019-present Saúl Ibarra Corretgé <s@saghul.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef TJS_UTILS_H
#define TJS_UTILS_H

#include <quickjs.h>
#include <stdbool.h>
#include <stdlib.h>
#include <uv.h>


#ifndef countof
#define countof(x) (sizeof(x) / sizeof((x)[0]))
#endif

struct AssertionInfo {
    const char *file_line;  // filename:line
    const char *message;
    const char *function;
};

#define ERROR_AND_ABORT(expr)                                                                                          \
    do {                                                                                                               \
        static const struct AssertionInfo args = { __FILE__ ":" STRINGIFY(__LINE__), #expr, PRETTY_FUNCTION_NAME };    \
        tjs_assert(args);                                                                                              \
    } while (0)

#ifdef __GNUC__
#define LIKELY(expr)         __builtin_expect(!!(expr), 1)
#define UNLIKELY(expr)       __builtin_expect(!!(expr), 0)
#define PRETTY_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define LIKELY(expr)         expr
#define UNLIKELY(expr)       expr
#define PRETTY_FUNCTION_NAME ""
#endif

#define STRINGIFY_(x) #x
#define STRINGIFY(x)  STRINGIFY_(x)

#define CHECK(expr)                                                                                                    \
    do {                                                                                                               \
        if (UNLIKELY(!(expr))) {                                                                                       \
            ERROR_AND_ABORT(expr);                                                                                     \
        }                                                                                                              \
    } while (0)

#define CHECK_EQ(a, b)      CHECK((a) == (b))
#define CHECK_GE(a, b)      CHECK((a) >= (b))
#define CHECK_GT(a, b)      CHECK((a) > (b))
#define CHECK_LE(a, b)      CHECK((a) <= (b))
#define CHECK_LT(a, b)      CHECK((a) < (b))
#define CHECK_NE(a, b)      CHECK((a) != (b))
#define CHECK_NULL(val)     CHECK((val) == NULL)
#define CHECK_NOT_NULL(val) CHECK((val) != NULL)

void tjs_assert(const struct AssertionInfo info);

#define TJS_CONST(x) JS_PROP_INT32_DEF(#x, x, JS_PROP_CONFIGURABLE)

uv_loop_t *tjs_get_loop(JSContext *ctx);
int tjs_obj2addr(JSContext *ctx, JSValueConst obj, struct sockaddr_storage *ss);
JSValue tjs_addr2obj(JSContext *ctx, const struct sockaddr *sa);
void tjs_call_handler(JSContext *ctx, JSValueConst func);
void tjs_dump_error(JSContext *ctx);
void tjs_dump_error1(JSContext *ctx, JSValueConst exception_val, bool is_throw);
void JS_FreePropEnum(JSContext *ctx, JSPropertyEnum *tab, uint32_t len);

typedef struct {
    JSValue p;
    JSValue rfuncs[2];
} TJSPromise;

JSValue TJS_InitPromise(JSContext *ctx, TJSPromise *p);
void TJS_FreePromise(JSContext *ctx, TJSPromise *p);
void TJS_FreePromiseRT(JSRuntime *rt, TJSPromise *p);
void TJS_ClearPromise(JSContext *ctx, TJSPromise *p);
void TJS_MarkPromise(JSRuntime *rt, TJSPromise *p, JS_MarkFunc *mark_func);
void TJS_SettlePromise(JSContext *ctx, TJSPromise *p, bool is_reject, int argc, JSValueConst *argv);
void TJS_ResolvePromise(JSContext *ctx, TJSPromise *p, int argc, JSValueConst *argv);
void TJS_RejectPromise(JSContext *ctx, TJSPromise *p, int argc, JSValueConst *argv);
JSValue TJS_NewResolvedPromise(JSContext *ctx, int argc, JSValueConst *argv);
JSValue TJS_NewRejectedPromise(JSContext *ctx, int argc, JSValueConst *argv);

#endif