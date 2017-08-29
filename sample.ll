; ModuleID = 'sample.dc'
source_filename = "sample.dc"

declare i32 @printnum(i32)

define i32 @test(i32 %j_arg) {
entry:
  %mul_tmp = mul i32 %j_arg, 10
  ret i32 %mul_tmp
}

define i32 @main() {
entry:
  %call_tmp = call i32 @test(i32 10)
  %call_tmp1 = call i32 @printnum(i32 %call_tmp)
  ret i32 0
}
