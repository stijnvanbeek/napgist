#pragma once

#include <Gist.h>
#include <audio/core/audionode.h>
#include <audio/core/audionodemanager.h>

#include <mutex>

namespace nap
{

    namespace audio
    {

        class GistNode : public Node
        {
        public:
            GistNode(NodeManager& nodeManager) : Node(nodeManager), mGist(nodeManager.getInternalBufferSize(), nodeManager.getSampleRate()) { }

            InputPin mAudioInput = { this };

            // Onset detection functions
            SampleValue getEnergyDifference()
            {
                std::lock_guard<std::mutex> lock(mMutex);
                return mGist.energyDifference();
            }

            SampleValue getPeakEnergy()
            {
                std::lock_guard<std::mutex> lock(mMutex);
                return mGist.peakEnergy();
            }

            float getPitch()
            {
                std::lock_guard<std::mutex> lock(mMutex);
                return mGist.pitch();
            }

            SampleValue getSpectralDifference() { return mGist.spectralDifference(); }
            SampleValue getSpectralDifferenceHWR() { return mGist.spectralDifferenceHWR(); }
            SampleValue getComplexSpectralDifference() { return mGist.complexSpectralDifference(); }
            SampleValue getHighFrequencyContent() { return mGist.highFrequencyContent(); }

        private:
            void process() override
            {
                auto inputBuffer = mAudioInput.pull();
                if (inputBuffer != nullptr)
                {
                    std::lock_guard<std::mutex> lock(mMutex);
                    mGist.processAudioFrame(*inputBuffer);
                }
            }

            void sampleRateChanged(float sampleRate) override { mGist.setSamplingFrequency(sampleRate); }
            void bufferSizeChanged(int bufferSize) override { mGist.setAudioFrameSize(bufferSize); }

            Gist<SampleValue> mGist;
            std::mutex mMutex;
        };

    }

}