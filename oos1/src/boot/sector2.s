[BITS 32]
[extern _main0]

[extern __main]  	;"_main()"������support.c��
[extern __atexit]	; "_atexit()"������support.c��

global greatstart
greatstart:
	mov eax,1
	mov eax,2
	mov eax,3

;Makefile��g++ѡ�� -nostartfiles��ֹ��g++ȥ����startup code,
;startup code�����ڽ���������C++��д��main0()����֮ǰ���Լ�main0()
;�˳�ʱִ�еĴ��룬��ִ�еĹ����ǳ�ʼ��(/����)global/static����
	call __main		;call our own startup code
	jmp _main0
	call __atexit  	;call our own startup code
