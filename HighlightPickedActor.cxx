#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMath.h>
#include <vtkNamedColors.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkPropAssembly.h>
#include <vtkAssemblyPath.h>

namespace {
    // Handle mouse events

    class MouseInteractorHighLightActor : public vtkInteractorStyleTrackballCamera {
    public:
        static MouseInteractorHighLightActor* New();
        vtkTypeMacro(MouseInteractorHighLightActor,
                vtkInteractorStyleTrackballCamera);

        MouseInteractorHighLightActor() {
            LastPickedActor = NULL;
            LastPickedProperty = vtkProperty::New();
        }

        virtual ~MouseInteractorHighLightActor() {
            LastPickedProperty->Delete();
        }

        virtual void OnLeftButtonDown() override {
            auto colors = vtkSmartPointer<vtkNamedColors>::New();

            int* clickPos = this->GetInteractor()->GetEventPosition();

            // Pick from this location.
            auto picker =
                    vtkSmartPointer<vtkPropPicker>::New();
            if(this->parts)
                picker->PickProp(clickPos[0], clickPos[1], this->GetDefaultRenderer(), this->parts);
            else
                picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

            // If we picked something before, reset its property
            if (this->LastPickedActor) {
                this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
            }
            auto pickedPath = picker->GetPath();
            this->LastPickedActor = pickedPath ? (vtkActor*)pickedPath->GetLastNode()->GetViewProp() : NULL;
            if (this->LastPickedActor) {
                // Save the property of the picked actor so that we can
                // restore it next time
                this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
                // Highlight the picked actor by changing its properties
                this->LastPickedActor->GetProperty()->SetColor(
                        colors->GetColor3d("Red").GetData());
                this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
                this->LastPickedActor->GetProperty()->SetSpecular(0.0);
                this->LastPickedActor->GetProperty()->EdgeVisibilityOn();
            }

            // Forward events
            vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        }

        void setParts(vtkPropCollection * parts) {
            this->parts = parts;
        }

    private:
        vtkActor* LastPickedActor;
        vtkProperty* LastPickedProperty;
        vtkPropCollection* parts;
    };
}

vtkStandardNewMacro(MouseInteractorHighLightActor);

vtkSmartPointer<vtkPropAssembly> createAssembly(
    const int numberOfParts,
    vtkNamedColors * colors
) {
    auto assembly =
            vtkSmartPointer<vtkPropAssembly>::New();
    for (int i = 0; i < numberOfParts; ++i) {
        auto source =
                vtkSmartPointer<vtkCubeSource>::New();
        double x, y, z, size;
        x = vtkMath::Random(-5, 5);
        y = vtkMath::Random(-5, 5);
        z = vtkMath::Random(-5, 5);
        size = vtkMath::Random(0.5, 1.0);
        source->SetXLength(size);
        source->SetYLength(size);
        source->SetZLength(size);
        source->SetCenter(x, y, z);
        auto mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(source->GetOutputPort());
        auto actor =
                vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        double r, g, b;
        r = vtkMath::Random(0.4, 1.0);
        g = vtkMath::Random(0.4, 1.0);
        b = vtkMath::Random(0.4, 1.0);
        actor->GetProperty()->SetDiffuseColor(r, g, b);
        actor->GetProperty()->SetDiffuse(0.8);
        actor->GetProperty()->SetSpecular(0.5);
        actor->GetProperty()->SetSpecularColor(
                colors->GetColor3d("White").GetData());
        actor->GetProperty()->SetSpecularPower(30.0);
        assembly->AddPart(actor);
    }
    return assembly;
}

// Execute application.

int main(int argc, char* argv[]) {
    auto colors =
            vtkSmartPointer<vtkNamedColors>::New();

    colors->SetColor("Bkg", 0.3, 0.4, 0.5);

    int numberOfSpheres = 3;
    if (argc > 1) {
        numberOfSpheres = atoi(argv[1]);
    }
    // A renderer and render window
    auto renderer =
            vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(640, 480);
    renderWindow->AddRenderer(renderer);

    // An interactor
    auto renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Set the custom type to use for interaction.
    auto style =
            vtkSmartPointer<MouseInteractorHighLightActor>::New();
    style->SetDefaultRenderer(renderer);

    renderWindowInteractor->SetInteractorStyle(style);

    for (int i = 0; i < numberOfSpheres; ++i) {
        auto source =
                vtkSmartPointer<vtkSphereSource>::New();
        double x, y, z, radius;
        x = vtkMath::Random(-5, 5);
        y = vtkMath::Random(-5, 5);
        z = vtkMath::Random(-5, 5);
        radius = vtkMath::Random(0.5, 1.0);
        source->SetRadius(radius);
        source->SetCenter(x, y, z);
        source->SetPhiResolution(11);
        source->SetThetaResolution(21);
        auto mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(source->GetOutputPort());
        auto actor =
                vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        double r, g, b;
        r = vtkMath::Random(0.4, 1.0);
        g = vtkMath::Random(0.4, 1.0);
        b = vtkMath::Random(0.4, 1.0);
        actor->GetProperty()->SetDiffuseColor(r, g, b);
        actor->GetProperty()->SetDiffuse(0.8);
        actor->GetProperty()->SetSpecular(0.5);
        actor->GetProperty()->SetSpecularColor(
                colors->GetColor3d("White").GetData());
        actor->GetProperty()->SetSpecularPower(30.0);
        renderer->AddActor(actor);
    }

    auto assembly = createAssembly(numberOfSpheres, colors);
    renderer->AddActor(assembly);
    // uncomment the following line to enable cube pick
    //style->setParts(assembly->GetParts());

    renderer->SetBackground(colors->GetColor3d("Bkg").GetData());

    // Render and interact
    renderWindow->Render();
    renderWindowInteractor->Initialize();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
