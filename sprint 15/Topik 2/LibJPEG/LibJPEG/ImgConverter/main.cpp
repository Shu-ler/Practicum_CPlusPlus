#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>

#include <filesystem>
#include <string_view>
#include <iostream>

using namespace std;

namespace img_lib {
    enum class Format {
        UNKNOWN,
        PPM,
        JPEG
    };
}

using namespace img_lib;

class ImageFormatInterface {
public:
    virtual bool SaveImage(const Path& file, const Image& image) const = 0;
    virtual Image LoadImage(const Path& file) const = 0;
    virtual ~ImageFormatInterface() = default;
};

namespace FormatInterfaces {
    class PPM : public ImageFormatInterface {
    public:
        bool SaveImage(const Path& file, const Image& image) const override {
            return img_lib::SavePPM(file, image);
        }
        Image LoadImage(const Path& file) const override {
            return img_lib::LoadPPM(file);
        }
    };

    class JPEG : public ImageFormatInterface {
    public:
        bool SaveImage(const Path& file, const Image& image) const override {
            return img_lib::SaveJPEG(file, image);
        }
        Image LoadImage(const Path& file) const override {
            return img_lib::LoadJPEG(file);
        }
    };
}

Format GetFormatByExtension(const Path& input_file) {
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return Format::JPEG;
    }

    if (ext == ".ppm"sv) {
        return Format::PPM;
    }

    return Format::UNKNOWN;
}

const ImageFormatInterface* GetFormatInterface(const Path& path) {
    static const FormatInterfaces::PPM ppmInterface;
    static const FormatInterfaces::JPEG jpegInterface;

    switch (GetFormatByExtension(path)) {
        case Format::PPM:
            return &ppmInterface;
        case Format::JPEG:
            return &jpegInterface;
        default:
            return nullptr;
    }
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return 1;
    }

    Path in_path = argv[1];
    Path out_path = argv[2];

    const ImageFormatInterface* input_format = GetFormatInterface(in_path);
    if (!input_format) {
        cerr << "Unknown format of the input file"sv << endl;
        return 2;
    }

    const ImageFormatInterface* output_format = GetFormatInterface(out_path);
    if (!output_format) {
        cerr << "Unknown format of the output file"sv << endl;
        return 3;
    }

    Image image = input_format->LoadImage(in_path);
    if (!image) {
        cerr << "Loading failed"sv << endl;
        return 4;
    }

    if (!output_format->SaveImage(out_path, image)) {
        cerr << "Saving failed"sv << endl;
        return 5;
    }

    cout << "Successfully converted"sv << endl;
    return 0;
}