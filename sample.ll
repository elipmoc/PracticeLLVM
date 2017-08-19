; ModuleID = 'sample.dc'
source_filename = "sample.dc"

define i32 @test(i32 %j_arg) {
entry:
  %mul_tmp = mul i32 %j_arg, 10
  ret i32 %mul_tmp
}

define i32 @main() {
entry:
  %call_tmp = call i32 @test(i32 10)
  ret i32 0
}
