import numpy as np
import vtk
import matplotlib.pyplot as plt
import os

reference_solution = "/home/lukas/Documents/SimTech/Semester2/NumSim/Submission1/scenario3_output_fixed/out/"
current_solution = (
    "/home/lukas/Documents/SimTech/Semester2/NumSim/Submission1/NumSim/build/out/"
)


def vti_to_numpy(vti_file_path):
    # Read the .vti file
    field_dictionary = {
        "u": None,
        "v": None,
        "p": None,
        "abs_vel": None,
        "dimension": None,
        "mesh_width": None,
    }
    reader = vtk.vtkXMLImageDataReader()
    reader.SetFileName(vti_file_path)
    reader.Update()

    # Get the VTK image data
    image_data = reader.GetOutput()

    # Get the dimensions of the image
    field_dictionary["dims"] = image_data.GetDimensions()
    field_dictionary["mesh_width"] = image_data.GetSpacing()

    # Convert VTK array to NumPy array
    np_array = np.array(image_data.GetPointData().GetArray(1))

    field_dictionary["u"] = np_array[:, 0].reshape(
        field_dictionary["dims"][1], field_dictionary["dims"][0]
    )
    field_dictionary["v"] = np_array[:, 1].reshape(
        field_dictionary["dims"][1], field_dictionary["dims"][0]
    )
    field_dictionary["p"] = np_array[:, 2].reshape(
        field_dictionary["dims"][1], field_dictionary["dims"][0]
    )

    field_dictionary["abs_vel"] = np.sqrt(
        field_dictionary["u"] * field_dictionary["u"]
        + field_dictionary["v"] * field_dictionary["v"]
    )

    return field_dictionary


def plot_field(plot_dictionary):
    scale_factor_x = 1.0
    scale_factor_y = 1.0

    if plot_dictionary["mesh_width"][0] > plot_dictionary["mesh_width"][1]:
        scale_factor_x = (
            plot_dictionary["mesh_width"][0] / plot_dictionary["mesh_width"][1]
        )
    else:
        scale_factor_y *= (
            plot_dictionary["mesh_width"][1] / plot_dictionary["mesh_width"][0]
        )

    extent = [
        0,
        plot_dictionary["field"].shape[1] * scale_factor_x,
        0,
        plot_dictionary["field"].shape[0] * scale_factor_y,
    ]

    fig, ax = plt.subplots()
    im = ax.imshow(
        plot_dictionary["field"][::-1, :],
        cmap="viridis",
        extent=extent,
        interpolation="bilinear",
    )
    ax.set_title("hello there")
    ax.set_xlabel("x-Direction")

    print(plot_dictionary["mesh_width"])

    fig.colorbar(im, ax=ax, label="abs")

    plt.show()


def extract_number_from_filename(filename):
    # Extracts the numeric part from a file name
    return int("".join(filter(str.isdigit, filename)))


def find_highest_number_filename(folder_path):
    # List all files in the folder
    files = os.listdir(folder_path)

    # Filter out only files (not directories)
    files = [f for f in files if os.path.isfile(os.path.join(folder_path, f))]

    if not files:
        return None, None  # No files in the folder

    # Extract numbers from file names and find the maximum
    max_file = max(files, key=extract_number_from_filename)
    highest_number = extract_number_from_filename(max_file)

    return max_file, highest_number


def make_plottable_field(input_field, field_to_plot="abs_vel"):
    plottable_field = {
        "field": input_field[field_to_plot],
        "mesh_width": input_field["mesh_width"],
    }

    return plottable_field


def get_difference_field(ref_field, current_field, field_to_compare="abs_vel"):
    assert ref_field[field_to_compare].shape == current_field[field_to_compare].shape
    difference_field = {
        "field": np.abs(ref_field[field_to_compare] - current_field[field_to_compare]),
        "mesh_width": current_field["mesh_width"],
    }

    return difference_field


if __name__ == "__main__":
    max_file_ref, highest_number_ref = find_highest_number_filename(reference_solution)
    max_file_current, highest_number_current = find_highest_number_filename(
        current_solution
    )

    current_field = vti_to_numpy(current_solution+"output_"+'{:04d}'.format(highest_number_current)+".vti")
    reference_field = vti_to_numpy(reference_solution+"output_"+'{:04d}'.format(highest_number_ref)+".vti")

    assert current_field["u"].shape == reference_field["u"].shape
    
    diff_field = get_difference_field(reference_field, current_field, "v")

    plot_field(diff_field)
