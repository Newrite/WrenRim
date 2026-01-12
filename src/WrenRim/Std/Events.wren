class Events {
    static on(event, fn) {
        if (__listeners == null) __listeners = {}
        var list = __listeners[event]
        if (list == null) {
            list = []
            __listeners[event] = list
        }
        list.add(fn)
    }

    static clear() {
        __listeners = {}
    }

    static dispatch(event) {
        if (__listeners) {
            var list = __listeners[event]
            if (list) {
                for (fn in list) fn.call()
            }
        }
    }

    static dispatch(event, a1) {
        if (__listeners) {
            var list = __listeners[event]
            if (list) {
                for (fn in list) fn.call(a1)
            }
        }
    }

    static dispatch(event, a1, a2) {
        if (__listeners) {
            var list = __listeners[event]
            if (list) {
                for (fn in list) fn.call(a1, a2)
            }
        }
    }

    static dispatch(event, a1, a2, a3) {
        if (__listeners) {
            var list = __listeners[event]
            if (list) {
                for (fn in list) fn.call(a1, a2, a3)
            }
        }
    }

    static dispatch(event, a1, a2, a3, a4) {
        if (__listeners) {
            var list = __listeners[event]
            if (list) {
                for (fn in list) fn.call(a1, a2, a3, a4)
            }
        }
    }

    static dispatch(event, a1, a2, a3, a4, a5) {
        if (__listeners) {
            var list = __listeners[event]
            if (list) {
                for (fn in list) fn.call(a1, a2, a3, a4, a5)
            }
        }
    }

    static dispatch(event, a1, a2, a3, a4, a5, a6) {
        if (__listeners) {
            var list = __listeners[event]
            if (list) {
                for (fn in list) fn.call(a1, a2, a3, a4, a5, a6)
            }
        }
    }

    static dispatch(event, a1, a2, a3, a4, a5, a6, a7) {
        if (__listeners) {
            var list = __listeners[event]
            if (list) {
                for (fn in list) fn.call(a1, a2, a3, a4, a5, a6, a7)
            }
        }
    }

    static dispatch(event, a1, a2, a3, a4, a5, a6, a7, a8) {
        if (__listeners) {
            var list = __listeners[event]
            if (list) {
                for (fn in list) fn.call(a1, a2, a3, a4, a5, a6, a7, a8)
            }
        }
    }
}
