
import iceoryx_binding_python

A = [1.,2.,3.,4.]

B = iceoryx_binding_python.modify(A)
print(B)

print("square() working", iceoryx_binding_python.square(10))

str = "ssstringgg"
iceoryx_binding_python.test(str)

ret = iceoryx_binding_python.initRuntime("app_name")
print(ret)


