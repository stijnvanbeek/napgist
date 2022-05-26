#pragma once

#include <Gist.h>
#include <audio/core/audionode.h>
#include <audio/core/audionodemanager.h>

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
            SampleValue getEnergyDifference() { return mGist.energyDifference(); }
            SampleValue getSpectralDifference() { return mGist.spectralDifference(); }
            SampleValue getSpectralDifferenceHWR() { return mGist.spectralDifferenceHWR(); }
            SampleValue getComplexSpectralDifference() { return mGist.complexSpectralDifference(); }
            SampleValue getHighFrequencyContent() { return mGist.highFrequencyContent(); }

        private:
            void process() override
            {
                auto inputBuffer = mAudioInput.pull();
                if (inputBuffer != nullptr)
                    mGist.processAudioFrame(*inputBuffer);
            }

            void sampleRateChanged(float sampleRate) override { mGist.setSamplingFrequency(sampleRate); }
            void bufferSizeChanged(int bufferSize) override { mGist.setAudioFrameSize(bufferSize); }

            Gist<SampleValue> mGist;
        };

    }

}