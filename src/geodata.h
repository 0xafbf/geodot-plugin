#ifndef __GEODATA_H__
#define __GEODATA_H__

#include <Godot.hpp>

#include "defines.h"
#include "geoimage.h"

// Forward declaration of OGRLayer
class OGRLayer;

namespace godot {

/// A dataset which contains layers of geodata.
/// Corresponds to GDALDataset.
class EXPORT GeoDataset : public Resource {
    GODOT_CLASS(GeoDataset, Resource)

public:
    GeoDataset();
    virtual ~GeoDataset();

    /// Automatically called by Godot
    void _init();
    static void _register_methods();

    /// Returns true if the GeoDataset could successfully be loaded.
    bool is_valid();

    /// Returns a GeoRasterLayer object of the layer within this dataset with the given name.
    /// It is recommended to check the validity of the returned object with GeoRasterLayer::is_valid().
    GeoRasterLayer get_raster_layer(String name);

    /// Returns a GeoFeatureLayer object of the layer within this dataset with the given name.
    /// It is recommended to check the validity of the returned object with GeoFeatureLayer::is_valid().
    GeoFeatureLayer get_feature_layer(String name);

    /// Load a dataset file such as a Geopackage or a Shapefile into this object.
    void load_from_file(String file_path);

    /// Set the GDALDataset object directly.
    /// Not exposed to Godot since Godot doesn't know about GDALDatasets - this is only for internal use.
    void set_gdaldataset(GDALDataset *new_dataset);

private:
    GDALDataset *dataset;
};

/// A layer which contains any number of features.
/// These features consist of attributes and usually (but not necessarily) vector geometry.
/// This layer provides access to these features through various filters.
/// Corresponds to OGRLayer.
class EXPORT GeoFeatureLayer : public Resource {
    GODOT_CLASS(GeoFeatureLayer, Resource)

public:
    GeoFeatureLayer();
    virtual ~GeoFeatureLayer();

    /// Automatically called by Godot
    void _init();
    static void _register_methods();

    /// Returns true if the layer could successfully be loaded.
    bool is_valid();

    /// Returns all features, regardless of the geometry, wtihin this layer.
    Array get_all_features();

    /// Returns features with line geometry near the given position (within the given radius).
    Array get_lines_near_position(double pos_x, double pos_y, double radius, int max_lines);

    /// Returns features with point geometry near the given position (within the given radius).
    Array get_points_near_position(double pos_x, double pos_y, double radius, int max_points);

    /// Crops features with line geometry to the square created by the given coordinates and size.
    /// Useful for doing tile-based requests.
    Array crop_lines_to_square(double top_left_x, double top_left_y, double size_meters, int max_lines);

    /// Load the first layer of the dataset at the given path into this object.
    /// Useful e.g. for simple shapefiles with only one layer.
    void load_from_file(String file_path);

    /// Set the OGRLayer object directly.
    /// Not exposed to Godot since Godot doesn't know about GDALDatasets - this is only for internal use.
    void set_ogrlayer(OGRLayer *new_layer);

private:
    OGRLayer *layer;
};

/// A layer which contains raster data.
/// Corresponds to a Raster GDALDataset or Subdataset.
class EXPORT GeoRasterLayer : public Resource {
    GODOT_CLASS(GeoRasterLayer, Resource)

public:
    GeoRasterLayer();
    virtual ~GeoRasterLayer();

    /// Automatically called by Godot
    void _init();
    static void _register_methods();

    /// Returns true if the layer could successfully be loaded.
    bool is_valid();

    Ref<GeoImage> get_image(double top_left_x, double top_left_y, double size_meters,
                            int img_size, int interpolation_type);
    
    /// Load a raster dataset file such as a GeoTIFF into this object.
    void load_from_file(String file_path);

    /// Set the GDALDataset object for this layer. Must be a valid raster dataset.
    /// Not exposed to Godot since Godot doesn't know about GDALDatasets - this is only for internal use.
    void set_gdaldataset(GDALDataset *new_dataset);

private:
    GDALDataset *dataset;
};

}

#endif // __GEODATA_H__
