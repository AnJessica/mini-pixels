#include "duckdb/main/extension/generated_extension_loader.hpp"

namespace duckdb{

//! Looks through the CMake-generated list of extensions that are linked into DuckDB currently to try load <extension>
bool TryLoadLinkedExtension(DuckDB &db, const string &extension) {

    if (extension=="parquet") {
        db.LoadStaticExtension<ParquetExtension>();
        return true;
    }
    if (extension=="tpch") {
        db.LoadStaticExtension<TpchExtension>();
        return true;
    }
    if (extension=="pixels") {
        db.LoadStaticExtension<PixelsExtension>();
        return true;
    }
    if (extension=="jemalloc") {
        db.LoadStaticExtension<JemallocExtension>();
        return true;
    }

    return false;
}

const vector<string>& LinkedExtensions(){
    static vector<string> VEC = {
	"parquet",
	"tpch",
	"pixels",
	"jemalloc"
    };
    return VEC;
}


const vector<string>& LoadedExtensionTestPaths(){
    static vector<string> VEC = {
    };
    return VEC;
}

}
