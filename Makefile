CFLAGS = -lubsan -D NDEBUG -g -std=c++14 -fmax-errors=1 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++0x-compat -Wc++11-compat -Wc++14-compat  -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion  -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline  -Wlogical-op -Wmissing-declarations  -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstack-usage=8192  -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types  -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef  -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation  -fstack-protector -fstrict-overflow   -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -fsanitize=address  -fsanitize=bool -fsanitize=bounds -fsanitize=enum  -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -lm -pie

all:
	g++ syntax/*.cpp trans/*.cpp tree_simp/*.cpp tree/*.cpp general/*.cpp text_processing/*.cpp main.cpp logs/*.cpp lang_common/*.cpp lexic/*.cpp -O0 -o temp.exe -lubsan -D DEBUG -g -std=c++14 -fmax-errors=1 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++0x-compat -Wc++11-compat -Wc++14-compat -Wcast-align  -Wchar-subscripts -Wconditionally-supported -Wconversion  -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline  -Wlogical-op -Wmissing-declarations  -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstack-usage=8192  -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types  -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef  -Wunreachable-code -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation  -fstack-protector -fstrict-overflow   -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -fsanitize=address  -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum  -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -lm -pie -Wno-unused-parameter -std=c++17

logfile:
	>> /tmp/calc_logs.html

.PHONY: asm proc imagesdir procexecute asmexecute

asm:
	g++ proc/assembler/*.cpp text_processing/*.cpp general/*.cpp logs/*.cpp -o proc/asm.exe -lubsan -D NDEBUG -g -std=c++14 -fmax-errors=1 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++0x-compat -Wc++11-compat -Wc++14-compat  -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion  -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline  -Wlogical-op -Wmissing-declarations  -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstack-usage=8192  -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types  -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef  -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation  -fstack-protector -fstrict-overflow   -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -fsanitize=address  -fsanitize=bool -fsanitize=bounds -fsanitize=enum  -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -lm -pie

proc:
	g++ proc/cpu/*.cpp proc/stack/*.cpp logs/*.cpp general/*.cpp text_processing/*.cpp proc/assembler/assembler.cpp -o proc/cpu.exe -lubsan -D NDEBUG -g -std=c++14 -fmax-errors=1 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++0x-compat -Wc++11-compat -Wc++14-compat  -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion  -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline  -Wlogical-op -Wmissing-declarations  -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstack-usage=8192  -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types  -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef  -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation  -fstack-protector -fstrict-overflow   -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -fsanitize=address  -fsanitize=bool -fsanitize=bounds -fsanitize=enum  -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -lm -pie

imagesdir:
	mkdir /tmp/diff_images 

procexecute:
	./proc/cpu.exe proc/text_files/circle.code proc/text_files/proc_input.txt proc/text_files/proc_output.txt  proc_logs.html

asmexecute:
	./proc/asm.exe proc/text_files/circle.txt proc/text_files/circle.code asm_logs.html proc/text_files/asm_listing.txt