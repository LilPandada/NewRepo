#include"VideoController.h"

#include <astra/capi/astra.h>
#include<astra/Vector2i.hpp>
#include<astra/Vector2f.hpp>
#include <stdio.h>
#include <key_handler.h>




astra_vector2f_t out;

vector<astra_vector2f_t> outlist;
astra_vector2i_t tem;
void print_hand_frame(astra_handframe_t handFrame)
{
    astra_handpoint_t* handPoints;
    uint32_t handCount;

    astra_handframe_get_hand_count(handFrame, &handCount);

    handPoints = (astra_handpoint_t*)malloc(handCount * sizeof(astra_handpoint_t));
    astra_handframe_copy_hands(handFrame, handPoints);

    astra_frame_index_t frameIndex;
    astra_handframe_get_frameindex(handFrame, &frameIndex);

    int activeHandCount = 0;
   /* int p = 0;
    int q = 0;
    int r = 0;*/

    int t = 0;
    int m = 0;
    float depthScale = 1920/1080;
    astra_handpoint_t* point = handPoints;
    
    
    int i;
    for (i = 0; i < handCount; i++)
    {
        if (point->status == HAND_STATUS_TRACKING)
        {
            ++activeHandCount;

            t = point->depthPosition.x;
            m = point->depthPosition.y;

            const float circleX = (t + .5f) * depthScale;
            const float circleY = (m + .5f) * depthScale;
            out.x = circleX;
            out.y = circleY;
            
            VideoController::getVideoController()->setXY(circleX, circleY);
            

           /* p = point->worldPosition.x;
            q = point->worldPosition.y;
            r = point->worldPosition.z;*/
            outlist.push_back(out);

           // printf("ºá×ø±ê%f,×Ý×ø±ê%f,   ", circleX, circleY);

           // printf("ºá×ø±ê%d,×Ý×ø±ê%d,Z×ø±ê%d    ",p,q,r);
            
        }
        ++point;
    }

    VideoController::getVideoController()->play();
    cout << ".." << endl;

   /* for (int i = 0; i < outlist.size(); i++) {
        printf("ºá×ø±ê%f,×Ý×ø±ê%f,   ", outlist[i].x, outlist[i].y);
    }*/
    free(handPoints);
    //printf("index %d active hand count %d\n", frameIndex, activeHandCount);
}

void runHandStream(astra_reader_t reader)
{
    char key = 0;
    do
    {
        astra_update();

        astra_reader_frame_t frame;
        astra_status_t rc = astra_reader_open_frame(reader, 0, &frame);

        if (rc == ASTRA_STATUS_SUCCESS)
        {
            astra_handframe_t handFrame;
            astra_frame_get_handframe(frame, &handFrame);

            print_hand_frame(handFrame);

            astra_colorframe_t handDebugimageframe;
            astra_frame_get_debug_handframe(frame, &handDebugimageframe);

            astra_image_metadata_t metadata;
            astra_colorframe_get_metadata(handDebugimageframe, &metadata);

            astra_reader_close_frame(&frame);
        }

        key = cv::waitKey(30);

    } while (key != 27);
}

int main(int argc, char* argv[])
{
    set_key_handler();

    astra_initialize();

    astra_streamsetconnection_t sensor;

    astra_streamset_open("device/default", &sensor);

    astra_reader_t reader;
    astra_reader_create(sensor, &reader);

    astra_handstream_t handStream;
    astra_reader_get_handstream(reader, &handStream);
    astra_stream_start(handStream);

    astra_colorstream_t handDebugimagestream;
    astra_reader_get_debug_handstream(reader, &handDebugimagestream);
    astra_stream_start(handDebugimagestream);

    runHandStream(reader);
   
    astra_reader_destroy(&reader);
    astra_streamset_close(&sensor);

    astra_terminate();
}