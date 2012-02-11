#include <stdlib.h> // for exit()
#include <cstring>
#include "ncAudioLoader.h"
#include "ncServiceLocator.h"

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

ncAudioLoader::ncAudioLoader(const char *pFilename)
	: m_fDuration(0.0f), m_ulNumSamples(0L), m_iChannels(0), m_iFrequency(0)
{
	Load(pFilename);
}

ncAudioLoader::~ncAudioLoader()
{
	ov_clear(&m_oggFile);
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

/// Loads audio data from file
void ncAudioLoader::Load(const char *pFilename)
{
	char *pDotChar = strrchr(const_cast<char *>(pFilename), '.');

	// A dot followed by at least three extension characters
	if (!pDotChar || strlen(pDotChar) < 4)
	{
		ncServiceLocator::GetLogger().Write(ncILogger::LOG_FATAL, (const char *)"ncAudioLoader::Load - No extension for file \"%s\"", pFilename);
		exit(-1);
	}

	if (!strncmp(pDotChar, ".ogg", 4))
		LoadOgg(pFilename);
	else
	{
		ncServiceLocator::GetLogger().Write(ncILogger::LOG_FATAL, (const char *)"ncAudioLoader::Load - Extension unknown \"%s\"", pDotChar);
		exit(-1);
	}
}

/// Decodes audio data in a specified buffer
/*! \param pBuffer Buffer pointer
 *  \param iBufSize Buffer size
 *  \return Number of bytes read.
 */
long ncAudioLoader::Read(char *pBuffer, int iBufSize) const
{
	static int iBitStream = 0;
	long lBytes;
	long int lBufSeek = 0;

	do {
		// Read up to a buffer's worth of decoded sound data
#ifdef __ANDROID__
		lBytes = ov_read(&m_oggFile, pBuffer+lBufSeek, iBufSize-lBufSeek, &iBitStream);
#else
		// 0 - little endian, 2 - 16bit, 1 - signed
		lBytes = ov_read(&m_oggFile, pBuffer+lBufSeek, iBufSize-lBufSeek, 0, 2, 1, &iBitStream);
#endif

		if (lBytes < 0)
		{
			ov_clear(&m_oggFile);
			ncServiceLocator::GetLogger().Write(ncILogger::LOG_FATAL, (const char *)"ncAudioLoader::Read - Error decoding at bitstream %d", iBitStream);
			exit(-1);
		}

		// Reset the static variable at the end of a decoding process
		if (lBytes <= 0)
			iBitStream = 0;

		lBufSeek += lBytes;
	}
	while (lBytes > 0 && iBufSize-lBufSeek > 0);

	return lBufSeek;
}

/// Resets the audio file seek value
void ncAudioLoader::Rewind() const
{
	ov_raw_seek(&m_oggFile, 0);
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void ncAudioLoader::LoadOgg(const char *pFilename)
{
	vorbis_info *pInfo;

	ncServiceLocator::GetLogger().Write(ncILogger::LOG_INFO, (const char *)"ncAudioLoader::LoadOGG - Loading \"%s\"", pFilename);

#ifdef __ANDROID__
	FILE *pFile = fopen(pFilename, "rb");

	if (ov_open(pFile, &m_oggFile, NULL, 0) != 0)
	{
		ncServiceLocator::GetLogger().Write(ncILogger::LOG_FATAL, (const char *)"ncAudioLoader::LoadOGG - Cannot open \"%s\" with ov_open", pFilename);
		fclose(pFile);
		exit(-1);
	}
#else
	if (ov_fopen(pFilename, &m_oggFile) != 0)
	{
		ncServiceLocator::GetLogger().Write(ncILogger::LOG_FATAL, (const char *)"ncAudioLoader::LoadOGG - Cannot open \"%s\" with ov_fopen", pFilename);
		exit(-1);
	}
#endif

	m_fDuration = float(ov_time_total(&m_oggFile, -1));
	m_ulNumSamples = ov_pcm_total(&m_oggFile, -1);

	// Get some information about the OGG file
	pInfo = ov_info(&m_oggFile, -1);

	m_iChannels = pInfo->channels;
	m_iFrequency = pInfo->rate;

	ncServiceLocator::GetLogger().Write(ncILogger::LOG_INFO, (const char *)"ncAudioLoader::LoadOGG - duration: %.2f, channels: %d, frequency: %d\n", m_fDuration, m_iChannels, m_iFrequency);
}