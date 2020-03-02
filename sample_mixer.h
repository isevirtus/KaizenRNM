namespace KaizenLibrary
{
	class RNode;

	class SampleMixer
	{
	public:
		SampleMixer();
		~SampleMixer();
		virtual double mix(KaizenLibrary::RNode *rNode) = 0;
	};

	class WMean :
		public SampleMixer
	{
	public:
		WMean();
		~WMean();
		double mix(KaizenLibrary::RNode *rNode);
	};

	class WMin :
		public SampleMixer
	{
	public:
		WMin();
		~WMin();
		double mix(KaizenLibrary::RNode *rNode);
	};

	class WMax :
		public SampleMixer
	{
	public:
		WMax();
		~WMax();
		double mix(KaizenLibrary::RNode *rNode);
	};

	class EmptyMixer :
		public SampleMixer
	{
	public:
		EmptyMixer();
		~EmptyMixer();
		double mix(KaizenLibrary::RNode *rNode);
	};
}