using System;

namespace Build
{
	/// <summary>
	/// error ��ժҪ˵����
	/// </summary>
	public class Error
	{
		public static int ErrorType = 0;

		public const int SUCCESS = 0;
		/// <summary>
		/// ���п�������
		/// </summary>
		public const int DATABLOCKOVERFLOW = 1;
		/// <summary>
		/// inode������
		/// </summary>
		public const int INODEOVERFLOW = 2;
		/// <summary>
		/// д���ļ�ʱ����Դ�ļ�����
		/// </summary>
		public const int ORIGFILEERROR = 3;
		/// <summary>
		/// ����Ŀ¼ʱ��û��������Ŀ¼��
		/// </summary>
		public const int SEARCHDIR = 4;
		/// <summary>
		/// ͬһĿ¼�µ��ļ�����ͬ
		/// </summary>
		public const int SAMEFILENAME = 5;

		public Error()
		{

		}
	}
}
