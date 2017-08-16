; ModuleID = 'sample.dc'
source_filename = "sample.dc"

define i32 @test(i32 %j_arg) {
entry:
  %j = alloca i32
  store i32 %j_arg, i32* %j
  %i = alloca i32
  %var_tmp = load i32, i32* %j
  %mul_tmp = mul i32 %var_tmp, 10
  store i32 %mul_tmp, i32* %i
  %var_tmp1 = load i32, i32* %i
  ret i32 %var_tmp1
}

define i32 @main() {
entry:
  %i = alloca i32
  store i32 10, i32* %i
  %var_tmp = load i32, i32* %i
  %call_tmp = call i32 @test(i32 %var_tmp)
  ret i32 0
}
