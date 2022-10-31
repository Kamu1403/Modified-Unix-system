//Support.c

/*
������ǰ����� extern ��C������Ȼ���ļ���support.c��.c��׺���ļ���
�ļ�����Ҳ��pure C code��������ΪMakefile�����support.c�����õ���
g++��������gcc�����Ժ�����ǰ�滹��Ҫ����extern ��C��������ᱨ
��undefined reference to __main()�����ƵĴ���
*/
extern "C" void _main()
{
	extern void (*_CTOR_LIST__)();   
	/*Note�� �˴�ʹ��_CTOR_LIST__��������prefix һ�� ��_��   
			��link.ld��Ҫprefix ���� ��_��*/
	
	void (**constructor)() = &_CTOR_LIST__;

	//the first element is the number of constructors
	int total = *(int *)constructor;
	
	//constructor++;   
		/*  (�����ȿ�һ�����ӽű���Link.ld)
		Link script���޸Ĺ��������total�Ѿ�����constructor�ĸ����ˣ�
		_CTOR_LIST__�ĵ�һ����Ԫ��ʼ����global/static�����constructor��
		���Բ��� constructor++; 
		*/
	
	while(total) //total����constructor���������������ڼ���Ƿ���_CTOR_LIST__��ĩβ
	{
		(*constructor)();
		//total--;
		constructor++;
		total = *(int *)constructor;
	}
}

extern "C" void _atexit()
{
	extern void (*_DTOR_LIST__)();
	
	void (**deconstructor)() = &_DTOR_LIST__;
	
	int total = *(int *)deconstructor;
	
	deconstructor++;
	
	while(total)
	{
		(*deconstructor)();
		//total--;
		deconstructor++;
		total = *(int *)deconstructor;
	}
}
